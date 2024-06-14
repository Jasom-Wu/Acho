package myModules;

import android.util.Log;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import java.util.HashMap;
import java.util.Locale;
import java.util.Objects;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.WebSocket;
import okhttp3.WebSocketListener;
import okio.ByteString;

public class MyWebSockManager {

    private static final HashMap<String, MyWebSocket> TopicMap = new HashMap<>();

    private MyWebSockManager() {
    }

    public static MyWebSocket getSocketByTopic(String topicName) {
        return MyWebSockManager.TopicMap.get(topicName);
    }

    public static void addSocketToGroup(MyWebSocket w, String topicName) {
        assert !Objects.equals(topicName, "");
        MyWebSockManager.TopicMap.put(topicName, w);
    }

    public static void discardSocketFromGroup(String topicName) {
        assert !Objects.equals(topicName, "");
        MyWebSockManager.TopicMap.remove(topicName);
    }

    public static void resumeAllSocket() {
        for (String key : MyWebSockManager.TopicMap.keySet()) {
            MyWebSocket item = MyWebSockManager.TopicMap.get(key);
            assert item != null;
            if (item.resumeConnect()) {
                Log.d("MyWebSockManager", String.format("resumeAllSocket: websocket by topic [%s] resumed.", item.TopicName));
            }
        }
    }

    public static class MyWebSocket extends AppCompatActivity {
        private WebSocket websocket;
        private OkHttpClient client;
        private Request request;
        private Handler handler;
        private WebSocketListener listener;
        private String BrokerURL = "";
        private String TopicName = "";
        private Boolean isOpen = false;

        public MyWebSocket(Handler handler) {
            this.handler = handler;
            this.client = new OkHttpClient();
        }

        public MyWebSocket() {
        }

        public String getBrokerURL() {
            return BrokerURL;
        }

        public void webSocketClose() {
            this.websocket.close(1000, "[Broker] Good Bye~");
        }

        public MyWebSocket start(String brokerIP, int port, String indexURL) {
            assert port > 0;
            this.BrokerURL = String.format(Locale.CHINA, "ws://%s:%d/%s", brokerIP, port, indexURL);
            assert !Objects.equals(this.BrokerURL, "");
            this.client = new OkHttpClient();
            this.request = new Request.Builder().url(this.BrokerURL).build();
            this.listener = new WebSocketListener() {
                @Override
                public void onOpen(@NonNull WebSocket webSocket, @NonNull okhttp3.Response response) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            isOpen = true;
                        }
                    });
                    handler.onOpen(webSocket, response);
                }

                @Override
                public void onMessage(@NonNull WebSocket webSocket, @NonNull String text) {
                    handler.onMessage(webSocket, text);
                }

                @Override
                public void onMessage(@NonNull WebSocket webSocket, @NonNull ByteString bytes) {
                    handler.onMessage(webSocket, bytes);
                }

                @Override
                public void onClosing(@NonNull WebSocket webSocket, int code, @NonNull String reason) {
                    handler.onClosed(webSocket, code, reason);
                }

                @Override
                public void onClosed(@NonNull WebSocket webSocket, int code, @NonNull String reason) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            isOpen = false;
                        }
                    });
                    Log.i("MyWebSock", "WebSocket 已关闭");
                }

                @Override
                public void onFailure(@NonNull WebSocket webSocket, @NonNull Throwable t, okhttp3.Response response) {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            isOpen = false;
                        }
                    });
                    Log.i("MyWebSock", "WebSocket 连接失败");
                }
            };
            this.websocket = client.newWebSocket(request, listener);
            return this;
        }

        public MyWebSocket subscribeTopic(String topicName) {
            if (!Objects.equals(topicName, "")) {
                this.TopicName = topicName;
                MyWebSockManager.addSocketToGroup(this, topicName);
                return this;
            } else {
                Log.i("MyWebSocket", "subscribeTopic: [ERROR] ");
            }
            return this;
        }

        public MyWebSocket unsubscribeTopic() {
            MyWebSockManager.discardSocketFromGroup(this.TopicName);
            this.TopicName = "";
            return this;
        }

        public Boolean getIsOpen() {
            return this.isOpen;
        }

        public Boolean resumeConnect() {
            if (!this.getIsOpen()) {
                this.websocket = this.client.newWebSocket(this.request, this.listener);
                return true;
            }
            return false;
        }

        public Boolean sendText(String text) {
            boolean res = false;
            if (!Objects.equals(text, "")) {
                res = this.websocket.send(text);
                this.handler.onSend(this.websocket,text);
            }
            return res;
        }

        public boolean sendBytes(ByteString bytes) {
            boolean res = false;
            if (bytes.size() > 0) {
                res = this.websocket.send(bytes);
                this.handler.onSend(this.websocket,bytes);
            }
            return res;
        }
    }

    public interface Handler {
        default void onMessage(WebSocket webSocket, String text) {
            Log.i("MyWebSockManager", "onMessage(text):" + text);
        }

        default void onMessage(WebSocket webSocket, ByteString bytes) {
            Log.i("MyWebSockManager", "onMessage(bytes)" + bytes.hex());
        }

        default void onClosed(WebSocket webSocket, int code, String reason) {
            Log.i("MyWebSockManager", "WebSocket 已关闭");
        }

        default void onOpen(WebSocket webSocket, okhttp3.Response response) {
            Log.i("MyWebSock", "WebSocket 连接成功");
        }
        default void onSend(WebSocket webSocket, String text){
            Log.i("MyWebSock", "onSend(text): "+"'"+text+"'");
        }
        default void onSend(WebSocket webSocket, ByteString bytes){
            Log.i("MyWebSock", "onSend(bytes): "+bytes.hex());
        }
    }

}
