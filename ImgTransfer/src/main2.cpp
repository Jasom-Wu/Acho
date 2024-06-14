#include "opencv2/opencv.hpp"
#include "iostream"
#include "fstream"
#include "imgProcess.h"
using namespace std;
using namespace cv;
//void getdata(vector<uchar>&data)
//{
//    data
//}
int main2()
{
/*//�ļ�������
//    ofstream fout("temp.txt");
//    assert(fout.is_open());
//    fout<<"hello"<<endl<<"this is a text"<<endl;
//    fout.close();
//    ifstream fin("../pic/endata.txt",ios::binary);
//    assert(fin.is_open());
//    stringstream buff;
////    buff<<fin.rdbuf();
//    fin>>buff.rdbuf();
//    cout<<buff.str()<<endl;
*/
/*//�ļ��������
    Mat img = imread("../pic/me.png",IMREAD_COLOR);
    if (img.empty()) {
        cout << "Error" << endl;
        return -1;
    }
    vector<uchar> endata;
    imencode(".png",img,endata);
    String str_endata(endata.begin(), endata.end());
    ofstream fout("../pic/endata.txt",ios::binary);
    assert(fout.is_open());
    fout<<str_endata;
    fout.close();
    imshow("Me", img);
    waitKey(1000);

//    imwrite("../pic/change_me.png",img);
    ifstream fin("../pic/endata.txt",ios::binary);
    assert(fin.is_open());
    stringstream str_temp;
    fin>>str_temp.rdbuf();
    fin.close();
    String  read_str = str_temp.str();
    vector<uchar> read_data(read_str.begin(), read_str.end());
    Mat Read_Mat;
    Read_Mat = imdecode(read_data,IMREAD_GRAYSCALE);
    imshow("�����",Read_Mat);
    waitKey();
    */
//����Ԫ�ط��ʼ���Ƭ
/*
    Mat img = imread("../pic/me.png");
    cout<< img.size<<endl;
    cout<< img.depth()<<endl;
    cout<< img.channels()<<endl;
    *img.ptr(1,1) = 0;
    img.at<Vec3b>(0,1)[2] = 1;
//    Mat temp_mat = Mat(img,Rect(0,0,2,2));
//    cout<< format(temp_mat,cv::Formatter::FMT_MATLAB)<<endl;
    cout<< format(img.rowRange(0,2).colRange(0,2),cv::Formatter::FMT_MATLAB)<<endl;
*/
//    for i=1:hei
//    for j=1:wid
//    if IM(i,j) > 128
//    err = IM(i,j)-255;
//    IM(i,j) = 255;
//    else
//    err = IM(i,j);
//    IM(i,j) = 0;
//    end
//    if i+1<= hei && j+1<=wid
//    IM(i,j+1) = IM(i,j+1) + err*3/8;
//    IM(i+1,j) = IM(i+1,j) + err*3/8;
//    IM(i+1,j+1) = IM(i+1,j+1) + err*1/4;
//    end
//            end
//    end

//    Mat img_read = imread("../pic/me.png",IMREAD_COLOR);
//    vector<Mat> img_chanels;
//    split(img_read,img_chanels);
//    Mat img = 0.299*img_chanels[0]+0.587*img_chanels[1]+0.114*img_chanels[2];
//    imshow("Merged",img);
////    Mat img = img_read;
//    float err = 0;
//    img.convertTo(img,CV_32F);
//#define Limitation(x,min,max) (x<min?min:x>max?max:x)
//    for(int32_t r=0;r<img.rows;r++)
//    {
//        for(int32_t c=0;c<img.cols;c++)
//        {
//            if(img.at<int>(r,c)>128)
//            {
//                err = (float)img.at<float>(r,c) -255;
//                img.at<float>(r,c)=255;
//            } else{
//                err = (float)img.at<float>(r,c);
//                img.at<float>(r,c)=0;
//            }
//            if(r+1<img.rows&&c+1<img.cols&&c-1>=0)
//            {
//                img.at<float>(r,c+1)+=err*7/16;
//                img.at<float>(r+1,c)+=err*5/16;
//                img.at<float>(r+1,c+1)+=err*1/16;
//                img.at<float>(r+1,c-1)+=err*3/16;
////                img.at<uchar>(r,c+1) = Limitation(img.at<uchar>(r,c+1)+err*3/8,0,255);
////                img.at<uchar>(r+1,c) = Limitation(img.at<uchar>(r+1,c)+err*3/8,0,255);
////                img.at<uchar>(r+1,c+1) = Limitation(img.at<uchar>(r+1,c+1)+err*2/8,0,255);
//            }
//        }
//    }
//    img.convertTo(img,CV_8U);
//    imshow("Dither",img);
//    waitKey();
//    char* data = (char*)malloc(100);
//    string t = "gggg";
//    strcpy_s(data,100,t.data());
//    printf("%s",data);
    lv_img_header_t out;
    getImgArray("../pic/me.png",out);
    Mat uu(out.h,out.w,CV_8UC1);
    memcpy_s(uu.data,uu.total(),out.data,out.w*out.h);
    imshow("asf",uu);
    waitKey();
    return 0;
}