//
// Created by Mr.WU on 2023/9/14.
//

#include "broker_app.h"

///////////////////////////////移植映射修改区////////////////////////////////////////////
static FIL *file = (FIL *) &SDFile;        //为file申请内存
////////////////////////////////////////////////////////////////////////////////////////


static void fileDownload(char *payload, uint16_t size, void *des_path) {
    uint8_t res;
    UINT count=0;
    if (size > 0) {
        if(HandlerManager.need_reset==0){
            if(file->fs==0){
                res = f_open(file, (const TCHAR *) des_path, FA_CREATE_ALWAYS | FA_WRITE);
                if (res)return;
            }
        } else{
            res = f_open(file, (const TCHAR *) des_path, FA_CREATE_ALWAYS | FA_WRITE);
            if (res)return;
        }
        f_write(file, payload, size, &count);
        if(HandlerManager.need_reset==0){
            if(size<RXBUFFERSIZE)
                f_close(file);
            else
                HandlerManager.run(HandlerManager.timeout_ms,1); // proceed to receive the un-ending data
        } else{
            f_close(file);
        }
    }
}

void downLoadFileList(File_TypeTypeDef type,UartHandle_FinishCallBackFuncTypedef callback_fun,void *callback_data) {
    char *path = NULL;
    if (type == IMG) {
        path = "img_list.txt";
    } else if (type == AUDIO) {
        path = "audio_list.txt";
    }
    Printf("get_file_list:%d", type);
    HandlerManager.mount(fileDownload,callback_fun,path,callback_data)->run(5000,1);
}

void downLoadFile(File_TypeTypeDef type,char * file_id,char * file_name,UartHandle_FinishCallBackFuncTypedef callback_fun,void *callback_data) {
    char * path_buff = (char*)global_buff512;
    if(type==IMG){
        char *dot = strrchr(file_name, '.'); // 查找最后一个点的位置
        if (dot)*dot = '\0';
        snprintf(path_buff, 50, "Images\\%s.bin",file_name);
        Printf("get_dither_bin:%s", file_id);
        HandlerManager.mount(fileDownload,callback_fun,path_buff,callback_data)->run(7000,1);
    }
    else if(type==AUDIO){
        snprintf(path_buff, 50, "Audios\\%s",file_name);
        Printf("get_audio_file:%s", file_id);
        HandlerManager.mount(fileDownload,callback_fun,path_buff,callback_data)->run(7000,0);
    } else if(type==QR){
        char *dot = strrchr(file_name, '.'); // 查找最后一个点的位置
        if (dot)*dot = '\0';
        snprintf(path_buff, 50, "Setting\\%s.bin",file_name);
        Printf("get_bind_QR");
        HandlerManager.mount(fileDownload,callback_fun,path_buff,callback_data)->run(7000,1);
    }
}



FILE_Info_StateTypedef loadNextFileData(FIL *file_ptr, File_InfoTypedef *file_info) {
    if(file_ptr==NULL||file_info ==NULL||f_error(file_ptr)!=FR_OK)return FILE_INFO_ERROR;
    memset(file_info, 0, sizeof(File_InfoTypedef));
    uint8_t stack_buff[FILE_INFO_STACK_SIZE]={0};
    uint16_t stack_index=0;
    uint8_t info_flag = 0;
    static uint32_t file_index=0;
    static uint16_t buff_index=0;
    static uint8_t sector_flag=0;
    static uint8_t last_sector_flag =0;
    UINT br;
    if(file_ptr->fptr == 0){
        memset(global_buff512, 0, 512);
        sector_flag = last_sector_flag = buff_index = file_index = 0;
        if (f_read(file_ptr, global_buff512, sizeof(uint8_t) * 512, &br))return FILE_INFO_ERROR;
    }
    while(1){
        if(last_sector_flag!=sector_flag){
            if(sector_flag==1){
                memset(global_buff512, 0, 256);
                if (f_read(file_ptr, global_buff512, sizeof(uint8_t) * 256, &br))return FILE_INFO_ERROR;
            }
            else{
                memset(&global_buff512[256], 0, 256);
                if (f_read(file_ptr, &global_buff512[256], sizeof(uint8_t) * 256, &br))return FILE_INFO_ERROR;
            }
            last_sector_flag=sector_flag;
        }
        if(global_buff512[buff_index]=='\t' || global_buff512[buff_index]=='\n'){
            switch (info_flag) {
                case 0:{
                    memcpy(file_info->id,stack_buff,sizeof(uint8_t)*FILE_INFO_ID_SIZE-1);
                    file_info->id[FILE_INFO_ID_SIZE-1] = '\0';
                    info_flag = 1;
                    break;
                }
                case 1:{
                    memcpy(file_info->name,stack_buff,sizeof(uint8_t)*FILE_INFO_NAME_SIZE-1);
                    file_info->name[FILE_INFO_NAME_SIZE-1] = '\0';
                    info_flag = 2;
                    break;
                }
                case 2:{
                    memcpy(file_info->description,stack_buff, sizeof(uint8_t)*FILE_INFO_DESC_SIZE-1);
                    file_info->description[FILE_INFO_DESC_SIZE-1] = '\0';
                    info_flag = 3;
                    break;
                }
                default:
                    break;
            }
            memset(stack_buff,0, sizeof(stack_buff));
            stack_index = 0;
            buff_index++;
            if(info_flag==3){
                info_flag = 0;
                return FILE_INFO_OK;
            }
        } else{
            stack_buff[stack_index++] = global_buff512[buff_index++];
        }
        if(++file_index>file_ptr->fsize&& f_eof(file_ptr)){
            sector_flag = last_sector_flag = buff_index = file_index = 0;
            return FILE_INFO_END;
        }
        if(stack_index>=FILE_INFO_STACK_SIZE){
            memset(stack_buff,0, sizeof(uint8_t)*FILE_INFO_STACK_SIZE);
            stack_index = 0;
        }
        if(buff_index>=512)buff_index=0;
        sector_flag = buff_index>=256?1:0;
    }
}

