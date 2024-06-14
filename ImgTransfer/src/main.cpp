#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include "imgProcess.h"
#include "filesInfoGet.h"
#include "opencv2/opencv.hpp"
#include <windows.h>
#include<direct.h>
using namespace cv;
#define PATH_MAX_SIZE 30
#define GUI_SHOW_IMAGE 1

#if GUI_SHOW_IMAGE ==1
#include <opencv2/highgui/highgui.hpp>
int win_width=500,win_height=500;
Mat image,win_image;       //����ȫ�ֱ���
Rect rect_win,rect_img;

void moveImage()//ʵ���ƶ�ͼ��
{
    Mat image_ROI=image(rect_img);  // ����Դͼ�����Ȥ����ROI����Ҫ��ʾ������
    image_ROI.convertTo(win_image,image_ROI.type());  // image_ROI ���Ƶ� win_image
    ////Ҳ����ֱ���� Mat win_image=image(rect_img); //���Ǻܿ� Why��
    imshow("INDEX",win_image);
}
void on_mouse( int event, int x, int y, int flags, void* ustc) //int x,int y���������λ�ڴ��ڵģ�x��y������λ�ã��������Ͻ�Ĭ��Ϊԭ�㣬����Ϊx�ᣬ����Ϊy��
{
    // static������̬�ֲ�������ֵ�ں������ý�������ʧ������ԭֵ��
    //����ռ�õĴ洢��Ԫ���ͷţ����´θú�������ʱ���ñ���������һ�κ������ý���ʱ��ֵ
    static Point p0;
    static int xrect_img;  //�������ʱ��������ʾͼ�����Ͻ���Դͼ����x��y
    static int yrect_img;
    if(event==EVENT_LBUTTONDOWN)
    {
        p0=Point(x,y); //��ȡ����������ʱ����ʼ��
        xrect_img=rect_img.x;
        yrect_img=rect_img.y;
    }
    if(event==EVENT_MOUSEMOVE&& (flags & EVENT_LBUTTONDOWN)) //������£�����ƶ�ʱ
    {
        int dx=x-p0.x;
        int dy=y-p0.y;
        if(x>=0 && x<=win_width-1 && y>=0 && y<=win_height-1) //�ж�����Ƿ��ڴ���ͼ��������
        {
            rect_img=Rect(xrect_img-dx,yrect_img-dy,rect_img.width,rect_img.height);  //������ʾͼ���ƶ�dx��dy�����������������ʱ��
            if(rect_img.x<0)
            {
                rect_img.x=0;
            }
            if(rect_img.y<0)
            {
                rect_img.y=0;
            }
            if(rect_img.x > image.cols-rect_img.width-1)
            {
                rect_img.x=image.cols-rect_img.width-1;
            }
            if(rect_img.y > image.rows - rect_img.height-1)
            {
                rect_img.y=image.rows - rect_img.height-1;
            }
            moveImage();
        }
    }
}
#endif



int main(int argc,char* argv[]) {
//    for(int i=0;i<argc;i++)
//    {
//        printf("��%d�������ǣ�%s\r\n",i+1,argv[i]);
//    }
    char *bin_output_path, *img_output_path, *src_path, *mode = nullptr;
    int len = (int) (strlen(argv[0]) + PATH_MAX_SIZE);
    bin_output_path = (char *) malloc(sizeof(char) * len);
    img_output_path = (char *) malloc(sizeof(char) * len);
    src_path = (char *) malloc(sizeof(char) * len);
    memset(bin_output_path, 0, len);
    memset(img_output_path, 0, len);
    memset(src_path, 0, len);
    if(argc>=2){
        mode = argv[1];
    }
//    if(mode && memcmp(mode,"-D",3)==0){
//        bool flag;
//        std::cout<< "Removing Directory 'bin_output'..  ";
//        flag = rmdir(".\\bin_output");
//        if(flag)std::cout<<"Successful!"<<std::endl; else std::cout<<"Failed!"<<std::endl;
//        std::cout<< "Removing Directory 'img_output'..  ";
//        flag = rmdir(".\\img_output");
//        if(flag)std::cout<<"Successful!"<<std::endl; else std::cout<<"Failed!"<<std::endl;
//        std::cout<< "Removing Directory 'img_src'..  ";
//        flag = rmdir(".\\img_src");
//        if(flag)std::cout<<"Successful!"<<std::endl; else std::cout<<"Failed!"<<std::endl;
//        return 0;
//    }


    strcat(bin_output_path, ".\\bin_output");
    strcat(img_output_path, ".\\img_output");
    strcat(src_path, ".\\img_src");
    if ((GetFileAttributesA(img_output_path) & FILE_ATTRIBUTE_DIRECTORY))CreateDirectory(bin_output_path, nullptr);
    if ((GetFileAttributesA(img_output_path) & FILE_ATTRIBUTE_DIRECTORY))CreateDirectory(img_output_path, nullptr);
    if ((GetFileAttributesA(src_path) & FILE_ATTRIBUTE_DIRECTORY)) {
        bool flag = CreateDirectory(src_path, nullptr);
        if (flag) {
            std::cout
                    << "Initialization Complete! \r\nLet's Add Some Images To 'img_src' Folder And Hit .exe Again To Start Converting."
                    << std::endl;
            getchar();
            return 0;
        }
    }
    std::cout << "Convert Begin..." << std::endl;

    File_Typedef *files_list;

    FILE *file = nullptr;
    lv_img_header_t header;
    long files_counts;
    long valid_counts = 0;
    char *open_buff = (char *) malloc(sizeof(char) * len);
    char *bin_output_buff = (char *) malloc(sizeof(char) * len);
    char *img_output_buff = (char *) malloc(sizeof(char) * len);
    uint8_t *data = nullptr;
    files_list = getImgsFileInfo(src_path, &files_counts);
    for (long i = 0; i < files_counts; i++) {
        if (memcmp(files_list[i].type, "png", 3) == 0 || memcmp(files_list[i].type, "jpg", 3) == 0) {
            std::cout << "Converting File '" << files_list[i].file_name << "'..." << "\t\t(Size: " << files_list[i].size
                      << " bytes" << std::endl;
            valid_counts++;
            memset(open_buff, 0, len);
            memset(bin_output_buff, 0, len);
            memset(img_output_buff, 0, len);
            sprintf(open_buff, "%s\\%s", src_path, files_list[i].file_name);
            sprintf(bin_output_buff, "%s\\%s", bin_output_path, files_list[i].file_name);
            sprintf(img_output_buff, "%s\\dither_%s", img_output_path, files_list[i].file_name);
            Mat ditherMat;
            if(mode && memcmp(mode,"-nodither", strlen("-nodither"))==0)
                ditherMat= getImgArray(open_buff, header, data, ENCODE_NO_DITHER_INDEX);
            else
                ditherMat= getImgArray(open_buff, header, data, ENCODE_DITHER_INDEX);
            imwrite(img_output_buff, ditherMat);
            char *pt = strstr(bin_output_buff, files_list[i].type);
            strcpy_s(pt, 4, "bin");
            std::ofstream fout(bin_output_buff, std::ios::binary);
            if (fout.is_open() != 1) {
                return -1;
            }
            fout.write((char *) &header, sizeof(lv_img_header_t));
            if (header.cf == 7)
                fout.write((char *) data, (header.h * ceil(header.w / 8.0) + 8) * sizeof(uint8_t));
            else
                fout.write((char *) data, (header.h * header.w) * sizeof(uint8_t));
            free(data);
            fout.close();
        }
    }
#if GUI_SHOW_IMAGE == 1
    free(files_list);
    files_list = getImgsFileInfo(bin_output_path,&files_counts);
    for(long i=0;i<files_counts;i++) {
        memset(open_buff,0,len);
        sprintf(open_buff,"%s\\%s",bin_output_path,files_list[i].file_name);
        if (memcmp(files_list[i].type, "bin", 3) == 0) {
            Mat tempmat = imgDecode(open_buff,header,DECODE_INDEX);
            image = tempmat;
            win_width = tempmat.cols>1500?1500:tempmat.cols-1;
            win_height = tempmat.rows>850?850:tempmat.rows-1;
            rect_img=Rect(0,0,win_width,win_height);
            Mat win_image=image(rect_img);
            imshow("INDEX",win_image);
            setMouseCallback("INDEX", on_mouse);
            waitKey();
        }
    }
#endif
    if (valid_counts == 0) {
        std::cout << "There's No Images Has Been Converted." << std::endl;
    } else {
        std::cout << valid_counts << " Image(s) In Total Has been Converted." << std::endl;
    }
    std::cout << "Convert Finished!" << std::endl;
    getchar();
    free(files_list);
    free(open_buff);
    free(bin_output_buff);
    free(src_path);
    free(bin_output_path);
    return 0;
}