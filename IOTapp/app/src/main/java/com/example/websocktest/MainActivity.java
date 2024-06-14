package com.example.websocktest;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;


import android.os.Debug;
import android.util.JsonReader;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.alibaba.fastjson.JSON;
import com.yzq.zxinglibrary.android.CaptureActivity;
import com.yzq.zxinglibrary.bean.ZxingConfig;
import com.yzq.zxinglibrary.common.Constant;

import org.json.JSONException;
import org.json.JSONObject;


import java.io.IOException;
import java.io.StringReader;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import okhttp3.Call;
import okhttp3.Response;
import okhttp3.WebSocket;
import myModules.MyHttpManager;
import myModules.MyWebSockManager;


public class MainActivity extends AppCompatActivity {

    public native String stringFromJNI();

    String Token = "";
    String ServerIP = "192.168.43.29";
    ActivityResultLauncher<Intent> activityScanLauncher;
    ActivityResultLauncher<Intent> activityLoginLauncher;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
//        Debug.waitForDebugger();
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.main_layout);

        EditText editText = findViewById(R.id.edit_text);
        editText.setKeyListener(null);
        EditText sock_msg_edit = findViewById(R.id.sock_msg_edit);
        TextView label1 = findViewById(R.id.label1);

        activityScanLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == Activity.RESULT_OK) {
                        Intent data = result.getData();
                        assert data != null;
                        String content = data.getStringExtra(Constant.CODED_CONTENT);
                        new MyHttpManager.HttpGet(new MyHttpManager.onRespond() {
                            @Override
                            public void SuccessfulCallBackFunc(Call call, Response response) throws IOException {
                                MyHttpManager.onRespond.super.SuccessfulCallBackFunc(call, response);
                                try {
                                    assert response.body() != null;
                                    com.alibaba.fastjson.JSONObject data = JSON.parseObject(response.body().string());
                                    String status = data.getString("status");
                                    if(status.equals("existed")){
                                        label1.setText("该设备之前已经绑定过了。");
                                        Toast.makeText(MainActivity.this, "This device had bind previously.", Toast.LENGTH_LONG).show();
                                    }
                                    else if(status.equals("successful")){
                                        label1.setText("成功绑定设备！");
                                        Toast.makeText(MainActivity.this, "Bind device successfully!", Toast.LENGTH_LONG).show();
                                    }
                                    else if(status.equals("QR-expired")){
                                        label1.setText("绑定失败，设备的二维码已过期");
                                        Toast.makeText(MainActivity.this, "Something wrong while binding.Failed!", Toast.LENGTH_LONG).show();
                                    }
                                    else{
                                        label1.setText("您还未登录，请登录后重试。");
                                        Toast.makeText(MainActivity.this, "Please login!", Toast.LENGTH_LONG).show();
                                    }
                                }catch (Exception e){
                                    Log.d("JSON-ERROR", "SuccessfulCallBackFunc: "+e);
                                }
                            }
                        }).start("http://" + ServerIP + ":8787/home/user/bind/?bind_token="+content+"&tok="+Token);
                    }
                }
        );
        activityLoginLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == Activity.RESULT_OK) {
                        Intent data = result.getData();
                        assert data != null;
                        Token = data.getStringExtra("TOKEN");
                        String user_name = data.getStringExtra("NAME");
                        label1.setText("已登录\n"+"Welcome!"+user_name);
                    }
                }
        );

        findViewById(R.id.button).setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity.this, CameraAlbumTest.class);
            activityScanLauncher.launch(intent);
        });

        findViewById(R.id.button0).setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity.this, CaptureActivity.class);
            /*ZxingConfig是配置类
             *可以设置是否显示底部布局，闪光灯，相册，
             * 是否播放提示音  震动
             * 设置扫描框颜色等
             * 也可以不传这个参数
             * */
            ZxingConfig config = new ZxingConfig();
            config.setPlayBeep(true);//是否播放扫描声音 默认为true
            config.setShake(true);//是否震动  默认为true
            config.setDecodeBarCode(true);//是否扫描条形码 默认为true
//            config.setReactColor(R.color.colorAccent);//设置扫描框四个角的颜色 默认为白色
//            config.setFrameLineColor(R.color.colorAccent);//设置扫描框边框颜色 默认无色
//            config.setScanLineColor(R.color.colorAccent);//设置扫描线的颜色 默认白色
            config.setFullScreenScan(false);//是否全屏扫描  默认为true  设为false则只会在扫描框中扫描
            intent.putExtra(Constant.INTENT_ZXING_CONFIG, config);
            activityScanLauncher.launch(intent);
        });

        findViewById(R.id.button1).setOnClickListener(v -> {
            Calendar calendar = Calendar.getInstance();
            int hour = calendar.get(Calendar.HOUR_OF_DAY);
            Intent intent = new Intent(MainActivity.this, UserActivity.class);
            intent.putExtra("DATA_H", hour);
            activityLoginLauncher.launch(intent);
        });

//        findViewById(R.id.button2).setOnClickListener(v -> new MyHttpManager.HttpGet(new MyHttpManager.onRespond() {
//            @Override
//            public void SuccessfulCallBackFunc(Call call, Response response) throws IOException {
//                try {
//                    assert response.body() != null;
//                    com.alibaba.fastjson.JSONObject data = JSON.parseObject(response.body().string());
//                    String status = data.getString("status");
//                    String text="500-ERROR!";
//                    if(!status.equals("failed")){
//                        if(status.equals("uncorrected")){
//                            text = "Password Error!";
//                        }
//                        else{
//                            Token = data.getString("token");
//                            text = "Welcome!";
//                        }
//                    }
//                    editText.setText(text);
//                }catch (Exception e){
//                    Log.d("JSON-ERROR", "SuccessfulCallBackFunc: "+e);
//                }
//            }
//            @Override
//            public void FailedCallBackFunc(Call call, Response response) {
//                editText.setText("Request ERR Code:"+response.code());
//            }
//        }).start("http://" + ServerIP + ":8787/welcome/password/check/?username=Jasom-Wu&password=www"));

        MyWebSockManager.MyWebSocket websocketInstance = new MyWebSockManager.MyWebSocket(new MyWebSockManager.Handler() {
            @Override
            public void onMessage(WebSocket webSocket, String text) {
                MyWebSockManager.Handler.super.onMessage(webSocket, text);
                editText.setText("\nReceive<" + text);
            }

            @Override
            public void onSend(WebSocket webSocket, String text) {
                MyWebSockManager.Handler.super.onSend(webSocket, text);
                editText.setText(editText.getText() + "\nSend>" + text);
            }
        });

        findViewById(R.id.button3).setOnClickListener(v -> {
            String msg = sock_msg_edit.getText().toString();
            if (!msg.equals("")) {
                sock_msg_edit.setHint("something");
                sock_msg_edit.setBackground(null);
                if (!websocketInstance.getIsOpen())
                    websocketInstance
                            .start(ServerIP, 8787, "broker/ws/subscribe/test_id_2/only_device/" + Token)
                            .subscribeTopic("test");
                try {
                    if (websocketInstance.getIsOpen())
                        websocketInstance.sendText(new JSONObject().put("message", msg).toString());
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            } else {
                GradientDrawable drawable = new GradientDrawable();
                drawable.setStroke(7, Color.RED);  // 设置边框宽度和颜色
                drawable.setCornerRadius(50);  // 设置边框圆角半径
                sock_msg_edit.setBackground(drawable);
                sock_msg_edit.setHint("input first!");
            }
        });
    }
}

