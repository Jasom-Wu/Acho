package myModules;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentUris;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Toast;

import androidx.activity.result.ActivityResult;
import androidx.activity.result.ActivityResultLauncher;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

public class MyAlbumManager {
    private static final int REQUEST_ALBUM_PERMISSION = 201;
    public static String currentImagePath;

    private MyAlbumManager() {
    }


    public static class MyAlbum {
        private Activity activity;
        private ActivityResultLauncher<Intent> activityLauncher;

        private MyAlbum() {
        }

        public MyAlbum(Activity activity, ActivityResultLauncher<Intent> launcher) {
            this.activity = activity;
            this.activityLauncher = launcher;
        }

        public void start() {
            if (ContextCompat.checkSelfPermission(activity, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
                openAlbum();
            } else {
                ActivityCompat.requestPermissions(activity,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUEST_ALBUM_PERMISSION);
            }
        }

        public void AlbumRequestCallBackFun(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
            if (requestCode == REQUEST_ALBUM_PERMISSION) {
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    openAlbum();
                } else {
                    Toast.makeText(activity, "Album permission denied", Toast.LENGTH_SHORT).show();
                }
            }
        }

        public void handleImage(ActivityResult result) {
            if (result.getResultCode() == Activity.RESULT_OK) {
                assert result.getData() != null;
                if (Build.VERSION.SDK_INT >= 19) {
                    handleImageOnKitKat(result.getData());
                } else {
                    handleImageBeforeKitKat(result.getData());
                }
            }
        }

        private void handleImageOnKitKat(Intent data) {
            currentImagePath = null;
            Uri uri = data.getData();
            Log.d("TAG", "handleImageOnKitKat: uri is " + uri);
            if (DocumentsContract.isDocumentUri(activity, uri)) {
                // 如果是document类型的Uri，则通过document id处理
                String docId = DocumentsContract.getDocumentId(uri);
                if ("com.android.providers.media.documents".equals(uri.getAuthority())) {
                    String id = docId.split(":")[1]; // 解析出数字格式的id
                    String selection = MediaStore.Images.Media._ID + "=" + id;
                    currentImagePath = getImagePath(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, selection);
                } else if ("com.android.providers.downloads.documents".equals(uri.getAuthority())) {
                    Uri contentUri = ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), Long.parseLong(docId));
                    currentImagePath = getImagePath(contentUri, null);
                }
            } else if ("content".equalsIgnoreCase(uri.getScheme())) {
                // 如果是content类型的Uri，则使用普通方式处理
                currentImagePath = getImagePath(uri, null);
            } else if ("file".equalsIgnoreCase(uri.getScheme())) {
                // 如果是file类型的Uri，直接获取图片路径即可
                currentImagePath = uri.getPath();
            }
        }

        private void handleImageBeforeKitKat(Intent data) {
            Uri uri = data.getData();
            currentImagePath = getImagePath(uri, null);
        }

        @SuppressLint("Range")
        private String getImagePath(Uri uri, String selection) {
            String path = null;
            // 通过Uri和selection来获取真实的图片路径
            Cursor cursor = activity.getContentResolver().query(uri, null, selection, null, null);
            if (cursor != null) {
                if (cursor.moveToFirst()) {
                    path = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
                }
                cursor.close();
            }
            return path;
        }

        private void openAlbum() {
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("image/*");
            activityLauncher.launch(intent);
        }
    }


}
