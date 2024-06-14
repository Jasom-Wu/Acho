package com.example.websocktest;

import androidx.appcompat.app.AppCompatActivity;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.BounceInterpolator;
import android.view.animation.CycleInterpolator;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

import org.opencv.objdetect.Objdetect;

import java.io.IOException;

import myModules.MyHttpManager;
import okhttp3.Call;
import okhttp3.Response;

public class UserActivity extends AppCompatActivity {
    ImageView imageView;
    TextView textView;
    TextView textHint;
    EditText usernameEditText;
    EditText passwordEditText;
    EditText passwordAgainEditText;
    LinearLayout LL1;
    LinearLayout LL2;
    Button signUp;
    Button signIn;
    final boolean MODE_SIGN_UP = false;
    final boolean MODE_SIGN_IN = true;
    boolean Mode = MODE_SIGN_IN;
    String ServerIP = "192.168.43.29";
    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.user_layout);
        imageView = findViewById(R.id.imageView);
        textView = findViewById(R.id.textView);
        textHint = findViewById(R.id.hint);
        usernameEditText = findViewById(R.id.EditName);
        passwordEditText = findViewById(R.id.EditPassword);
        passwordAgainEditText = findViewById(R.id.EditPasswordAgain);
        LL1 = findViewById(R.id.LL1);
        LL2 = findViewById(R.id.LL2);
        LL1.setTranslationX(dip2px(-500));
        signUp = findViewById(R.id.SignUp);
        signIn = findViewById(R.id.SignIn);
        Intent intent = getIntent();
        int hour = intent.getIntExtra("DATA_H", 12);
        if (hour >= 6 && hour <= 18) {
            imageView.setImageResource(R.drawable.good_morning_img);
            textView.setText("Morning");
        } else {
            imageView.setImageResource(R.drawable.good_night_img);
            textView.setText("Night");
        }

        usernameEditText.setOnFocusChangeListener((v, hasFocus) -> {
            if(!hasFocus){
                if(!usernameEditText.getText().toString().equals("")){
                    new MyHttpManager.HttpGet(new MyHttpManager.onRespond() {
                        @Override
                        public void SuccessfulCallBackFunc(Call call, Response response) {
                            try {
                                assert response.body() != null;
                                String jsonData = response.body().string();
                                com.alibaba.fastjson.JSONObject data = JSON.parseObject(jsonData);
                                String status = data.getString("status");
                                if(status.equals("Found"))
                                    usernameEditText.setCompoundDrawablesWithIntrinsicBounds(0, 0, R.drawable.ic_user_green_24dp, 0);
                                else{
                                    showValidationErrorAnimation(usernameEditText);
                                    showHintAnimation(String.format("User '%s' is not existed!",usernameEditText.getText().toString() ));
                                    usernameEditText.setCompoundDrawablesWithIntrinsicBounds(0, 0, R.drawable.ic_user_red_24dp, 0);
                                }
                            }catch (Exception e){
                                Log.d("JSON-ERROR", "SuccessfulCallBackFunc: "+e);
                            }
                        }
                    }).start("http://" + ServerIP + ":8787/welcome/user/check/?username="+usernameEditText.getText().toString());
                }
                else
                    usernameEditText.setCompoundDrawablesWithIntrinsicBounds(0, 0, R.drawable.ic_user_white_24dp, 0);
            }
        });
        signUp.setOnClickListener(v -> {
            boolean last_mode = Mode;
            Mode = MODE_SIGN_UP;
            if(last_mode == MODE_SIGN_IN){
                passwordAgainEditText.setText(null);
                signIn.setEnabled(false);
                LL1.setVisibility(View.VISIBLE);
                ObjectAnimator LL2MoveAnim = ObjectAnimator.ofFloat(LL2, "translationY", 0f, 60f);
                LL2MoveAnim.setInterpolator(new BounceInterpolator());
                LL2MoveAnim.setDuration(600);
                ObjectAnimator LL1MoveAnim = ObjectAnimator.ofFloat(LL1, "translationX", dip2px(-500f), 0f);
                LL1MoveAnim.setInterpolator(new BounceInterpolator());
                LL1MoveAnim.setDuration(400);
                AnimatorSet animatorSet = new AnimatorSet();
                animatorSet.play(LL2MoveAnim).before(LL1MoveAnim);
                animatorSet.start();
                new Handler().postDelayed(() -> {
                    signIn.setEnabled(true);
                },1000);
            }
            else{
                if(passwordEditText.getText().toString().equals(passwordEditText.getText().toString())){

                }else{
                    usernameEditText.setCompoundDrawablesWithIntrinsicBounds(0, 0, R.drawable.ic_user_green_24dp, 0);

                }
            }
        });
        signIn.setOnClickListener(v -> {
            boolean last_mode = Mode;
            Mode = MODE_SIGN_IN;
            if(last_mode == MODE_SIGN_UP){
                signUp.setEnabled(false);
                ObjectAnimator LL2MoveAnim = ObjectAnimator.ofFloat(LL2, "translationY", 0f, -60f);
                LL2MoveAnim.setInterpolator(new BounceInterpolator());
                LL2MoveAnim.setDuration(600);
                ObjectAnimator LL1MoveAnim = ObjectAnimator.ofFloat(LL1, "translationX", 0f, dip2px(500f));
                LL1MoveAnim.setInterpolator(new BounceInterpolator());
                LL1MoveAnim.setDuration(400);
                AnimatorSet animatorSet = new AnimatorSet();
                animatorSet.play(LL2MoveAnim).after(LL1MoveAnim);
                animatorSet.start();
                new Handler().postDelayed(() -> {
                    LL1.setVisibility(View.GONE);
                    signUp.setEnabled(true);
                },1000);
            }
            else{
                new MyHttpManager.HttpGet(new MyHttpManager.onRespond() {
                    @Override
                    public void SuccessfulCallBackFunc(Call call, Response response) throws IOException {
                        try {
                            assert response.body() != null;
                            String jsonData = response.body().string();
                            JSONObject data = JSON.parseObject(jsonData);
                            String status = data.getString("status");
                            if (status.equals("uncorrected")) {
                                showHintAnimation("Password incorrect!");
                                showValidationErrorAnimation(passwordEditText);
                                passwordEditText.setCompoundDrawablesWithIntrinsicBounds(0, 0, R.drawable.ic_lock_red_24dp, 0);
                            } else if (status.equals("corrected")) {
                                Intent intent = new Intent();
                                intent.putExtra("TOKEN", data.getString("token"));
                                intent.putExtra("NAME", usernameEditText.getText().toString());
                                setResult(Activity.RESULT_OK, intent);
                                finish();
                            }
                            else if(status.equals("invalid-user")){
                                showHintAnimation(String.format("User '%s' is not existed!",usernameEditText.getText().toString() ));
                                showValidationErrorAnimation(usernameEditText);
                            }
                        }catch (Exception e){
                            Log.d("JSON-ERROR", "SuccessfulCallBackFunc: "+e);
                        }
                    }
                }).start("http://" + ServerIP + ":8787/welcome/password/check/?username="+usernameEditText.getText().toString()+"&password="+passwordEditText.getText().toString());
            }
        });
        Animation scaleUpAnimation = AnimationUtils.loadAnimation(this, R.anim.button_scale_up);
        Animation scaleDownAnimation = AnimationUtils.loadAnimation(this, R.anim.button_scale_down);
        scaleDownAnimation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
            }

            @Override
            public void onAnimationEnd(Animation animation) {
            }

            @Override
            public void onAnimationRepeat(Animation animation) {
            }
        });
        signIn.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP) {
                signIn.startAnimation(scaleUpAnimation);
            } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                signIn.startAnimation(scaleDownAnimation);
            }
            return false;
        });
        signUp.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_UP) {
                signUp.startAnimation(scaleUpAnimation);
            } else if (event.getAction() == MotionEvent.ACTION_DOWN) {
                signUp.startAnimation(scaleDownAnimation);
            }
            return false;
        });
    }

    private int dip2px(float dpVale) {
        final float scale = getResources().getDisplayMetrics().density;
        return (int) (dpVale * scale + 0.5f);
    }

    private void showValidationErrorAnimation(EditText OBJ) {
        Drawable bg_last = OBJ.getBackground();
        // 创建一个颜色动画，将文本框的背景颜色更改为红色
        ObjectAnimator backgroundColorAnimator = ObjectAnimator.ofArgb(OBJ,
                "backgroundColor", Color.RED, Color.TRANSPARENT);
        backgroundColorAnimator.setDuration(500);
        // 创建一个晃动动画
        TranslateAnimation shakeAnimation = new TranslateAnimation(-10, 10, 0, 0);
        shakeAnimation.setDuration(100);
        shakeAnimation.setInterpolator(new CycleInterpolator(5)); // 晃动5次

        // 同时播放两个动画
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(backgroundColorAnimator);
        animatorSet.start();

        // 播放晃动动画
        OBJ.startAnimation(shakeAnimation);
        // 在动画结束后，重置文本框的颜色
        backgroundColorAnimator.addListener(new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                OBJ.setBackground(bg_last);
            }
        });
    }
    private void showHintAnimation(String hint){
        textHint.setText(hint);
        ObjectAnimator AlphaAnimator1 = ObjectAnimator.ofFloat(textHint,
                "alpha", 0,1);
        AlphaAnimator1.setDuration(200);
        AlphaAnimator1.setInterpolator(new AccelerateInterpolator());
        ObjectAnimator AlphaAnimator2 = ObjectAnimator.ofFloat(textHint,
                "alpha", 1,0);
        AlphaAnimator2.setDuration(200);
        AlphaAnimator2.setInterpolator(new AccelerateInterpolator());
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.play(AlphaAnimator2).after(2000).after(AlphaAnimator1).after(500);
        animatorSet.start();
    }
}