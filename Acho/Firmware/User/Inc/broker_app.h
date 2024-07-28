//
// Created by Mr.WU on 2023/9/14.
//

#ifndef INK_PAPER_BROKER_APP_H
#define INK_PAPER_BROKER_APP_H
#include "bsp_usart.h"
#include "usart.h"
#include "stdint.h"
#include "fatfs.h"
#include "audio.h"
#include "string.h"

#define FILE_INFO_ID_SIZE 5
#define FILE_INFO_NAME_SIZE 30
#define FILE_INFO_DESC_SIZE 100
#define FILE_INFO_TOTAL_SIZE (FILE_INFO_ID_SIZE + FILE_INFO_NAME_SIZE + FILE_INFO_DESC_SIZE)
#if FILE_INFO_TOTAL_SIZE > 512
#error Can't large than buff-size '512'
#endif

#define FILE_INFO_STACK_SIZE 120

typedef struct {
    char id[FILE_INFO_ID_SIZE];
    char name[FILE_INFO_NAME_SIZE];
    char description[FILE_INFO_DESC_SIZE];
} File_InfoTypedef;

typedef enum {
    OTHERS=0,
    IMG,
    AUDIO,
    QR,
}File_TypeTypeDef;

typedef enum {
    FILE_INFO_OK=0,
    FILE_INFO_END,
    FILE_INFO_ERROR
}FILE_Info_StateTypedef;

void downLoadFileList(File_TypeTypeDef type,UartHandle_FinishCallBackFuncTypedef callback_fun,void *callback_data);

void downLoadFile(File_TypeTypeDef type,char * file_id,char * file_name,UartHandle_FinishCallBackFuncTypedef callback_fun,void *callback_data);

FILE_Info_StateTypedef loadNextFileData(FIL *file_ptr, File_InfoTypedef *file_info);
#endif //INK_PAPER_BROKER_APP_H
