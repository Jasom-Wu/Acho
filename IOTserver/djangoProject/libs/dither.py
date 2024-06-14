import math
import cv2
import numpy as np
import ctypes

import qrcode


def reshapeImgForScreen(src, scr_size):
    scr_size = list(scr_size)
    des_size = list(src.shape[0:2])
    if scr_size[0] < src.shape[0] or scr_size[1] < src.shape[1]:
        if src.shape[0] > scr_size[1]:
            des_size[0] = scr_size[0]
            des_size[1] = math.ceil(scr_size[0] * src.shape[1] / src.shape[0])
        else:
            des_size[1] = scr_size[1]
            des_size[0] = math.ceil(scr_size[1] * src.shape[0] / src.shape[1])
    return cv2.resize(src, des_size[::-1])


def getQRCodeData(msg,size):
    qr = qrcode.QRCode(border=2, box_size=10, error_correction=qrcode.constants.ERROR_CORRECT_H)
    qr.add_data(msg)
    img = qr.make_image()
    temp = np.array(img.get_image())
    temp = np.where(temp == True, 255, 0).astype('float32')
    temp = reshapeImgForScreen(temp, size)
    for r in range(temp.shape[0]):
        for c in range(temp.shape[1]):
            if temp[r, c] > 128:
                temp[r, c] = 255
            else:
                temp[r, c] = 0
    return convertToByteForLVGL(temp)


# Dither Method
def ditherImg(src):
    img_gray = 0.1140 * src[:, :, 0] + 0.2989 * src[:, :, 1] + 0.58701 * src[:, :, 2]
    dithered_img = np.array(img_gray, dtype=float).round()
    for r in range(dithered_img.shape[0]):
        for c in range(dithered_img.shape[1]):
            if dithered_img[r, c] > 128:
                err = dithered_img[r, c] - 255
                dithered_img[r, c] = 255
            else:
                err = dithered_img[r, c]
                dithered_img[r, c] = 0
            if r + 1 < dithered_img.shape[0] and c + 1 < dithered_img.shape[1] and c - 1 >= 0:
                temp = dithered_img[r, c + 1] + err * 7 / 16
                dithered_img[r, c + 1] = 0 if temp < 0 else 255 if temp > 255 else temp
                temp = dithered_img[r + 1, c] + err * 5 / 16
                dithered_img[r + 1, c] = 0 if temp < 0 else 255 if temp > 255 else temp
                temp = dithered_img[r + 1, c + 1] + err * 1 / 16
                dithered_img[r + 1, c + 1] = 0 if temp < 0 else 255 if temp > 255 else temp
                temp = dithered_img[r + 1, c - 1] + err * 3 / 16
                dithered_img[r + 1, c - 1] = 0 if temp < 0 else 255 if temp > 255 else temp
    dithered_img = np.array(dithered_img, dtype=np.uint8)
    return dithered_img


# end of dither method

def imgShowInScreen(img_src, scr_size, bd_color=(0, 0, 0), bd_thick=1):
    assert scr_size[1] >= img_src.shape[1] and scr_size[0] >= img_src.shape[0]
    padding_width = (scr_size[1] - img_src.shape[1]) // 2
    padding_height = (scr_size[0] - img_src.shape[0]) // 2
    cv2.namedWindow('ImageWindow', cv2.WINDOW_NORMAL)
    cv2.resizeWindow('ImageWindow', scr_size[1], scr_size[0])
    cv2.rectangle(img_src, (bd_thick, bd_thick),
                  (img_src.shape[1] - bd_thick, img_src.shape[0] - bd_thick), bd_color,
                  bd_thick)
    padding_img = cv2.copyMakeBorder(img_src, padding_height, padding_height, padding_width, padding_width,
                                     cv2.BORDER_CONSTANT, value=255)
    cv2.imshow("ImageWindow", padding_img)
    cv2.waitKey(0)


class LvImgHeader(ctypes.Structure):
    _fields_ = [
        ('cf', ctypes.c_uint32, 5),
        ('always_zero', ctypes.c_uint32, 3),
        ('reserved', ctypes.c_uint32, 2),
        ('w', ctypes.c_uint32, 11),
        ('h', ctypes.c_uint32, 11),
    ]


def convertToByteForLVGL(src):
    header = LvImgHeader()
    header.cf = 7
    header.always_zero = 0
    header.reserved = 0
    header.h = src.shape[0]
    header.w = src.shape[1]
    step = math.ceil(src.shape[1] / 8.0)
    img_stream = [0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff] + [0] * (src.shape[0] * step)
    for i in range(0, src.shape[0]):
        row_pt = 8 + i * step  # 跳过前面的index，紧接着位操作
        for j in range(0, src.shape[1]):
            if src[i, j] == 255:
                pt = row_pt + math.floor(j / 8)
                img_stream[pt] = img_stream[pt] | (0x80 >> (j % 8))
    data_arr = bytearray(img_stream)
    data = bytes(data_arr)
    header_byte = bytes(header)
    return header_byte + data


# 读取图像，解决imread不能读取中文路径的问题
def cv_imread(file_path):
    cv_img = cv2.imdecode(np.fromfile(file_path, dtype=np.uint8), -1)
    return cv_img


def getDitherImgBinData(src_path, scr_size):
    if type(scr_size) == list and len(scr_size) == 2:
        img = cv_imread(src_path)
        resized_img = reshapeImgForScreen(img, scr_size)
        dithered_img = ditherImg(resized_img)
        data = convertToByteForLVGL(dithered_img)
        return data
    return None


if __name__ == '__main__':
    screen_size = (200, 200)
    img = cv2.imread("img.png", cv2.IMREAD_COLOR)
    resized_img = reshapeImgForScreen(img, screen_size)
    dithered_img = ditherImg(resized_img)
    data = convertToByteForLVGL(dithered_img)
    # imgShowInScreen(dithered_img, screen_size)

    with open('dithered.bin', 'wb') as f:
        f.write(data)
