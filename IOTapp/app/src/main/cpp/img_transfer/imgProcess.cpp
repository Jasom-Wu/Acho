#include "imgProcess.h"

using namespace std;
using namespace cv;
Mat resizeMatForScreen(const Mat &mat, Size scr){
    int resized_h,resized_w;
    float scale;
    Mat resizedMat = Mat(mat);
    if(mat.rows>scr.height||mat.cols>scr.width){
        if(mat.cols>mat.rows){
            scale = (float)scr.width/(float)mat.cols;
            resized_w = scr.width;
            resized_h = (int)((float)mat.rows * scale);
        }
        else{
            scale = (float)scr.height/(float)mat.rows;
            resized_h = scr.height;
            resized_w = (int)((float)mat.cols * scale);
        }
        resize(mat,resizedMat,Size(resized_w,resized_h));
    }
    return resizedMat;
}

void getImgArray(const Mat &img,lv_img_header_t &Output,uint8_t * &data,ENCODE_MODE encode_mode){
    if(encode_mode==ENCODE_DITHER)
    {
        data = (uchar*)malloc(sizeof(uchar)*img.total());
        memcpy(data,img.data,img.total());
        Output.h = img.rows;
        Output.w = img.cols;
        Output.cf = 4;//LV_IMG_CF_TRUE_COLOR
        Output.always_zero=0;
        Output.reserved=0;
    }
    else if(encode_mode==ENCODE_INDEX)
    {
        long step = ceil(img.cols/8.0);
        data = (uchar*)calloc((img.rows*step+8),sizeof(uchar));
        uchar index_2[]={0xff, 0xff, 0xff,0xff};
        uchar index_1[]={0x00, 0x00, 0x00,0xff};//��
        memcpy(data,index_1,4*sizeof(uchar));
        memcpy(data+4,index_2,4*sizeof(uchar));
        uchar *pt;
        for(long i=0;i<img.rows;i++)
        {
            pt = data+i*step+8;
            for(long j=0;j<img.cols;j++)
            {
                if(img.at<uchar>(i,j)==255)
                {
                    pt[(long)floor(j/8)] |= (0x80>>(j%8));
                }
            }
        }
        Output.h = img.rows;
        Output.w = img.cols;
        Output.cf = 7;//LV_IMG_CF_INDEXED_1BIT
        Output.always_zero=0;
        Output.reserved=0;
    }
}

Mat getDitherMat(const Mat &img_read)
{
//    Mat img_read = imread(address,IMREAD_COLOR);
    vector<Mat> img_chanels;
    split(img_read,img_chanels);
    Mat img = 0.299*img_chanels[0]+0.587*img_chanels[1]+0.114*img_chanels[2];
    float err = 0;
    img.convertTo(img,CV_32F);
    for(int32_t r=0;r<img.rows;r++)
    {
        for(int32_t c=0;c<img.cols;c++)
        {
            if(img.at<int>(r,c)>128)
            {
                err = (float)img.at<float>(r,c) -255;
                img.at<float>(r,c)=255;
            } else{
                err = (float)img.at<float>(r,c);
                img.at<float>(r,c)=0;
            }
            if(r+1<img.rows&&c+1<img.cols&&c-1>=0)
            {
                img.at<float>(r,c+1)+=err*7/16;
                img.at<float>(r+1,c)+=err*5/16;
                img.at<float>(r+1,c+1)+=err*1/16;
                img.at<float>(r+1,c-1)+=err*3/16;
            }
        }
    }
    img.convertTo(img,CV_8U);
    return img;
}

Mat imgDecode(const char *address, lv_img_header_t &output_header,DECODE_MODE decode_mode)
{
    const char *pt = strstr(address,".bin");
    pt++;
    if(strcmp(pt,"bin")!=0)
    {
        printf(" file \"%s\" is not a '.bin' file",address);
        assert(strcmp(pt,"bin")==0);
    }
    FILE* file = fopen(address,"rb");
    Mat result;
    uint8_t *data = nullptr;
    if (file ==nullptr)
    {
        printf("can not open file \"%s\"",address);
    }
    fread(&output_header,sizeof(lv_img_header_t),1,file);
        if(output_header.cf==7&&decode_mode==DECODE_INDEX)//LV_IMG_CF_INDEXED_1BIT
        {
            fseek(file,sizeof(lv_img_header_t),SEEK_SET);
            uint8_t index[2][4];
            fread(index[0],sizeof(index[0]),1,file);
            fread(index[1],sizeof(index[1]),1,file);
            long step = ceil(output_header.w/8.0);
            data = (uint8_t *)calloc((long)output_header.h*step,sizeof(uint8_t));
            fread(data,sizeof(uint8_t)*output_header.h*step,1,file);
            Mat temp_mat(output_header.h,output_header.w,CV_8UC3,Scalar(0,0,0));
            uint8_t  index_val;
            uint8_t  *data_pt;
            for(long i=0;i<temp_mat.rows;i++)
            {
                data_pt = data+i*step;
                for(long j=0;j<temp_mat.cols;j++)
                {
                    index_val = (data_pt[(long)floor(j/8)] & (0x80>>(j%8)))!=0;
                    temp_mat.at<Vec3b>(i,j)[0] = index[index_val][0];
                    temp_mat.at<Vec3b>(i,j)[1] = index[index_val][1];
                    temp_mat.at<Vec3b>(i,j)[2] = index[index_val][2];
                }
            }
            result = temp_mat;
//            imshow("INDEX",temp_mat);
//            waitKey();
        }
        else if(output_header.cf==4)//LV_IMG_CF_TRUE_COLOR
        {
            if(decode_mode==DECODE_DITHER)
            {
                fseek(file,sizeof(lv_img_header_t),SEEK_SET);
                data = (uint8_t *)malloc(sizeof(uint8_t)*output_header.h*output_header.w);
                fread(data,sizeof(uint8_t)*output_header.h*output_header.w,1,file);
                Mat temp_mat(output_header.h,output_header.w,CV_8UC1,Scalar(0));
                memcpy(temp_mat.data,data,output_header.w*output_header.h*sizeof(uint8_t));
//                imshow("DITHER",temp_mat);
//                waitKey();
                result = temp_mat;
            }
            else if(decode_mode==DECODE_RGB332)
            {
                data = (uint8_t *)malloc(sizeof(uint8_t)*output_header.h*output_header.w);
                fread(data,sizeof(uint8_t)*output_header.h*output_header.w,1,file);
                RGB332 temp332{};
                Mat temp_mat(output_header.h,output_header.w,CV_8UC3,Scalar(0,0,0));
                for(int k=0;k<temp_mat.rows;k++)
                {
                    for(int i=0;i<temp_mat.cols;i++)
                    {
                        temp332.full=data[k*temp_mat.cols+i];
                        temp_mat.at<Vec3b>(k,i)[2] = (temp332.R<<(8-3))+(0xff>>3);
                        temp_mat.at<Vec3b>(k,i)[1] = (temp332.G<<(8-3))+(0xff>>3);
                        temp_mat.at<Vec3b>(k,i)[0] = (temp332.B<<(8-2))+(0xff>>2);
                    }
                }
                result = temp_mat;
                imshow("RGB332",temp_mat);
                waitKey();
            }
        }
        if(data== nullptr)
        {
            result = Mat::zeros(output_header.h,output_header.w,CV_8UC1);
            printf("the decode fmt maybe wrong!");
        }
    fclose(file);
    free(data);
    return result;
}