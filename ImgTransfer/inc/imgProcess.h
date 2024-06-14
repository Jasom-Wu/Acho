#ifndef IMG_PROCESS_H_
#define IMG_PROCESS_H_

#include <cstdint>
#include <cassert>
#include "string"
#include "opencv2/opencv.hpp"

typedef enum ENCODE_MODE{
    ENCODE_DITHER=0,
    ENCODE_DITHER_INDEX,
    ENCODE_NO_DITHER_INDEX,
};
typedef enum DECODE_MODE{
    DECODE_INDEX=0,
    DECODE_DITHER,
    DECODE_RGB332
};
typedef union RGB332
{
    uint8_t full;
    struct {
        uint8_t B:2;
        uint8_t G:3;
        uint8_t R:3;
    };
};
typedef struct {//总共应写入4字节

    uint32_t cf : 5;          /*Color format: See `lv_img_color_format_t`*/
    uint32_t always_zero : 3; /*It the upper bits of the first byte. Always zero to look like a
                                 non-printable character*/

    uint32_t reserved : 2; /*Reserved to be used later*/
//    uint32_t w;
//    uint32_t h;
    uint32_t w : 11; /*Width of the image map*/
    uint32_t h : 11; /*Height of the image map*/
} lv_img_header_t;
_declspec(dllexport) cv::Mat getImgArray(const std::string& address,lv_img_header_t &Output,uint8_t * &data,ENCODE_MODE encode_mode=ENCODE_DITHER);
cv::Mat imgDecode(const char *address, lv_img_header_t &output_header,DECODE_MODE decode_mode=DECODE_DITHER);
#endif