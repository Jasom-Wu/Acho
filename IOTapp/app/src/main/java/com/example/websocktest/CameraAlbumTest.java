package com.example.websocktest;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.Imgproc;

import java.io.File;

import myModules.MyAlbumManager;
import myModules.MyCameraManager;
import okhttp3.internal.Util;

public class CameraAlbumTest extends AppCompatActivity {
    static {
        System.loadLibrary("native-lib");
    }
    public native void decodeIndexBin(long matPtr,String bin_path);
    public native void resizeMat(long matPtr,int width,int height);
    public native void ditherMat(long matPtr);
    public native String matBinaryOutPut(long matPtr,String outBinDir,String outBinName);
    private ImageView picture;
    private MyCameraManager.MyCamera camera;
    private MyAlbumManager.MyAlbum album;
    private String OUT_BIN_DIR;
    public Bitmap compressImageForScreen(Context context, Mat originalMat, double scale) {
        assert scale>0;
        // 获取屏幕尺寸和像素密度
        DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
        int screenWidth = displayMetrics.widthPixels;
        float screenDensity = displayMetrics.density;

        // 获取图像尺寸
        int originalWidth = originalMat.width();
        int originalHeight = originalMat.height();

        // 计算压缩比例，并进行图像压缩
        int targetWidth = (int) (screenWidth / screenDensity *scale); // 目标宽度为屏幕宽度的 dp 单位
        float scaleRatio = (float) targetWidth / originalWidth;
        int targetHeight = (int) (originalHeight * scaleRatio);

        Mat resizedMat = new Mat();
        Size targetSize = new Size(targetWidth, targetHeight);
        Imgproc.resize(originalMat, resizedMat, targetSize);

        // 将压缩后的图像转换为 Bitmap
        Bitmap resizedBitmap = Bitmap.createBitmap(resizedMat.cols(), resizedMat.rows(), Bitmap.Config.ARGB_8888);
        Utils.matToBitmap(resizedMat, resizedBitmap);

        return resizedBitmap;
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.camera_album_test_layout);
        OpenCVLoader.initDebug();
        OUT_BIN_DIR= CameraAlbumTest.this.getExternalFilesDir(null).getAbsolutePath() + "/"+"InkHelper/DitheredBin";

        Button takePhoto = findViewById(R.id.take_photo);
        Button choicePhoto = findViewById(R.id.choice_photo);
        picture = findViewById(R.id.picture);

        findViewById(R.id.load_bin).setOnClickListener(v -> {
            Mat targetMat= new Mat();
            decodeIndexBin(targetMat.getNativeObjAddr(),OUT_BIN_DIR+"/dithered_img.bin");
            Bitmap resizedBitmap = Bitmap.createBitmap(targetMat.cols(), targetMat.rows(), Bitmap.Config.ARGB_8888);
            Utils.matToBitmap(targetMat, resizedBitmap);
            picture.setImageBitmap(resizedBitmap);
        });
        ActivityResultLauncher<Intent> activityCameraLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == Activity.RESULT_OK) {
                        Mat targetMat = Imgcodecs.imread(MyCameraManager.currentPhotoPath,Imgcodecs.IMREAD_COLOR);
                        Mat resizedMat = new Mat();
                        targetMat.copyTo(resizedMat);
                        ditherMat(targetMat.getNativeObjAddr());
                        Bitmap processedBitmap = compressImageForScreen(CameraAlbumTest.this, targetMat,2.5);
                        picture.setImageBitmap(processedBitmap);
                        resizeMat(resizedMat.getNativeObjAddr(),200,200);
                        ditherMat(resizedMat.getNativeObjAddr());
                        if(matBinaryOutPut(resizedMat.getNativeObjAddr(),OUT_BIN_DIR,"dithered_img")==null)
                            Toast.makeText(CameraAlbumTest.this, "Bin files output failed!", Toast.LENGTH_SHORT).show();
                        else
                            Toast.makeText(CameraAlbumTest.this, "Photo saved to: " + MyCameraManager.currentPhotoPath, Toast.LENGTH_LONG).show();
                    } else {
                        Toast.makeText(CameraAlbumTest.this, "No Capture Action!", Toast.LENGTH_SHORT).show();
                    }
                }
        );
        camera = new MyCameraManager.MyCamera(CameraAlbumTest.this, activityCameraLauncher);
        takePhoto.setOnClickListener(v -> camera.start("InkHelper/Pictures"));

        ActivityResultLauncher<Intent> activityAlbumLauncher = registerForActivityResult(
                new ActivityResultContracts.StartActivityForResult(),
                result -> {
                    if (result.getResultCode() == Activity.RESULT_OK) {
                        album.handleImage(result);
                        BitmapFactory.Options options = new BitmapFactory.Options();
                        options.inSampleSize = 2; // 缩小一半
                        Bitmap bitmap = BitmapFactory.decodeFile(MyAlbumManager.currentImagePath, options);
                        Mat mat = new Mat();
                        Utils.bitmapToMat(bitmap, mat);
                        // 将图像转换为灰度图
                        Mat grayMat = new Mat();
                        Imgproc.cvtColor(mat, grayMat, Imgproc.COLOR_RGB2GRAY);
                        // 将灰度图转换回 Bitmap
                        Bitmap grayBitmap = Bitmap.createBitmap(grayMat.cols(), grayMat.rows(), Bitmap.Config.ARGB_8888);
                        Utils.matToBitmap(grayMat, grayBitmap);
                        picture.setImageBitmap(grayBitmap);
                    }
                }
        );
        album = new MyAlbumManager.MyAlbum(CameraAlbumTest.this, activityAlbumLauncher);
        choicePhoto.setOnClickListener(v -> album.start());
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        camera.CameraRequestCallBackFun(requestCode, permissions, grantResults);
        album.AlbumRequestCallBackFun(requestCode, permissions, grantResults);
    }
}







