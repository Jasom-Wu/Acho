package myModules;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;
import androidx.activity.result.ActivityResultLauncher;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class MyCameraManager {
    public static String currentPhotoForeName;
    public static String currentPhotoPath;
    private static final int REQUEST_CAMERA_PERMISSION = 200;
    private static final int CAMERA_TAKE_PHOTO = 1;
    private MyCameraManager(){}
    public static class MyCamera{
        private Activity activity;
        private String StorageDir;
        private ActivityResultLauncher<Intent> activityLauncher;

        private MyCamera(){}
        public MyCamera(Activity activity, ActivityResultLauncher<Intent> launcher){
            this.activity = activity;
            this.activityLauncher = launcher;
        }
        public void start(String customStorageDir){
            StorageDir = customStorageDir;
            if (ContextCompat.checkSelfPermission(activity, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) {
                dispatchTakePictureIntent(customStorageDir);
            } else {
                ActivityCompat.requestPermissions(activity,
                        new String[]{Manifest.permission.CAMERA}, REQUEST_CAMERA_PERMISSION);
            }
        }
        public void CameraRequestCallBackFun(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults){
            if (requestCode == REQUEST_CAMERA_PERMISSION) {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    dispatchTakePictureIntent(StorageDir);
                } else {
                    Toast.makeText(activity, "Camera permission denied", Toast.LENGTH_SHORT).show();
                }
            }
        }
        private void dispatchTakePictureIntent(String customStorageDir) {
            File outputImage = null;
            try {
                outputImage = createImageFile(customStorageDir);
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            if (outputImage != null) {
                Uri imageUri;
                if (Build.VERSION.SDK_INT >= 24) {
                    imageUri = FileProvider.getUriForFile(activity, "com.example.websocktest.fileprovider", outputImage);
                } else {
                    imageUri = Uri.fromFile(outputImage);
                }
                // 构建Intent并启动新Activity
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
                activityLauncher.launch(intent);
            }
        }

        private File createImageFile(String customStorageDir) throws IOException {
            String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault()).format(new Date());
            String imageFileName = "JPEG_" + timeStamp + "_";
            File fileEx = activity.getExternalFilesDir(null);
            String path = fileEx.getAbsolutePath() + "/"+customStorageDir;
//            String path = "/storage/emulated/0/" + customStorageDir;
            File storageDir = new File(path);
            if (!storageDir.exists()) {
                storageDir.mkdirs();
            }
            File imageFile = File.createTempFile(
                    imageFileName,  /* prefix */
                    ".jpg",         /* suffix */
                    storageDir      /* directory */
            );
            currentPhotoPath = imageFile.getAbsolutePath();
            currentPhotoForeName = imageFileName;
            return imageFile;
        }
    }

    public interface Handler {
        default void SuccessfulCallBackFunc(Context context,Bitmap bitmap){
            Toast.makeText(context, "Photo saved to: " + currentPhotoPath, Toast.LENGTH_LONG).show();
        }
        default void FailedCallBackFunc(Context context){
            Toast.makeText(context, "No Capture Action!", Toast.LENGTH_SHORT).show();
        }
    }
}
