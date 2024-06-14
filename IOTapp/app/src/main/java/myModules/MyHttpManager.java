package myModules;

import android.util.Log;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.IOException;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;


public class MyHttpManager {
    private MyHttpManager() {
    }

    public static class HttpPost extends AppCompatActivity {
        private onRespond r;

        public HttpPost(onRespond r) {
            this.r = r;
        }

        private HttpPost() {}

        public void start(String url, String JSON_String) {
            new Thread(() -> {
                try {
                    new JSONObject(JSON_String);
                } catch (JSONException e) {
                    e.printStackTrace();
                    return;
                }
                OkHttpClient client = new OkHttpClient();

                // 构建请求体
                MediaType mediaType = MediaType.parse("application/json; charset=utf-8");

                RequestBody body = RequestBody.create(mediaType, JSON_String);

                // 构建请求对象
                Request request = new Request.Builder()
                        .url(url)
                        .post(body)
                        .build();

                client.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onResponse(@NonNull Call call, @NonNull Response response) {
                        // 处理响应
                        runOnUiThread(() -> {
                            // 在此处更新 UI 元素
                            if (response.isSuccessful()) {
                                try {
                                    r.SuccessfulCallBackFunc(call, response);
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            } else {
                                r.FailedCallBackFunc(call, response);
                            }
                            response.close();
                        });
                    }

                    @Override
                    public void onFailure(@NonNull Call call, @NonNull IOException e) {
                        // 处理请求失败的情况
                        e.printStackTrace();
                    }
                });
            }).start();
        }
    }

    public static class HttpGet extends AppCompatActivity {
        private onRespond r;

        public HttpGet(onRespond r) {
            this.r = r;
        }

        private HttpGet() {}

        public void start(String url) {
            new Thread(() -> {
                //创建OkHttpClient对象
                OkHttpClient client = new OkHttpClient();
                //创建Request
                Request request = new Request.Builder()
                        .url(url)//访问连接
                        .get()
                        .build();
                //创建Call对象
                client.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onResponse(@NonNull Call call, @NonNull Response response) {
                        // 处理响应
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                // 在此处更新 UI 元素
                                if (response.isSuccessful()) {
                                    try {
                                        r.SuccessfulCallBackFunc(call, response);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                    }
                                } else {
                                    r.FailedCallBackFunc(call, response);
                                }
                            }
                        });
                    }

                    @Override
                    public void onFailure(@NonNull Call call, @NonNull IOException e) {
                        // 处理请求失败的情况
                        e.printStackTrace();
                    }
                });
            }).start();
        }
    }

    public interface onRespond {
        default void SuccessfulCallBackFunc(Call call, Response response) throws IOException{
            assert response.body() != null;
            Log.i("MyHttpManager", "[POST]SuccessfulCallBackFunc: " + "Request successful with code" + response.code());
        }
        default void FailedCallBackFunc(Call call, Response response){
                Log.i("MyHttpManager", "[POST]FailedCallBackFunc: " + "Request failed with code: " + response.code());
        }
    }
}
