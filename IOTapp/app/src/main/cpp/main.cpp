//
// Created by Mr.WU on 2023/7/30.
//

#include <jni.h>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include "img_transfer/imgProcess.h"
#include <android/log.h>
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_websocktest_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
bool dirExists(const std::string& path) {
    struct stat info{};
    return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_websocktest_CameraAlbumTest_decodeIndexBin(JNIEnv* env,jobject,jlong matPtr,jstring bin_path) {
    auto *img_originMat = (cv::Mat *)matPtr;
    const char* bin_path_cstr = env->GetStringUTFChars(bin_path, nullptr);
    lv_img_header_t header;
    *img_originMat = imgDecode(bin_path_cstr,header,DECODE_INDEX);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_websocktest_CameraAlbumTest_resizeMat(JNIEnv* env,jobject,jlong matPtr,jint width,jint height) {
    auto *img_originMat = (cv::Mat *)matPtr;
    *img_originMat = resizeMatForScreen(*img_originMat,cv::Size(width,height));
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_websocktest_CameraAlbumTest_ditherMat(JNIEnv* env,jobject,jlong matPtr) {
    auto *img_originMat = (cv::Mat *)matPtr;
    *img_originMat = getDitherMat(*img_originMat);
}
extern "C"
JNIEXPORT jstring JNICALL
        Java_com_example_websocktest_CameraAlbumTest_matBinaryOutPut(JNIEnv* env,jobject,jlong matPtr,jstring outBinDir,jstring outBinName) {
const char *out_bin_dir, *out_bin_name;
    out_bin_dir = env->GetStringUTFChars(outBinDir, nullptr);
    out_bin_name = env->GetStringUTFChars(outBinName, nullptr);
    auto *img_originMat = (cv::Mat *)matPtr;
    uint8_t *data = nullptr;
    lv_img_header_t header;
    std::string bin_output_buff = out_bin_dir + std::string("/")+std::string(out_bin_name)+std::string(".bin");
    getImgArray(*img_originMat,header,data,ENCODE_INDEX);
    int result =0;
    if (!dirExists(out_bin_dir))
        result = mkdir(out_bin_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(result==0){
        std::ofstream fout(bin_output_buff, std::ios::binary);
        if (fout.is_open() == 1) {
            fout.write((char *) &header, sizeof(lv_img_header_t));
            if (header.cf == 7u)
                fout.write((char *) data, (header.h * ceil(header.w / 8.0) + 8) * sizeof(uint8_t));
            else
                fout.write((char *) data, (header.h * header.w) * sizeof(uint8_t));
            fout.close();
        }
        else
        {
            __android_log_print(ANDROID_LOG_WARN, "CameraAlbumTest", "Can't open file in path   %s",bin_output_buff.c_str());
            result=1;
        }
    }
    else
        __android_log_print(ANDROID_LOG_WARN, "CameraAlbumTest", "Can't create dir in path   %s",out_bin_dir);
    free(data);
    if(result==0)
        return env->NewStringUTF(bin_output_buff.c_str());
    else
        return nullptr;
}
