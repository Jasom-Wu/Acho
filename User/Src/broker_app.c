//
// Created by Mr.WU on 2023/9/14.
//

#include "broker_app.h"

///////////////////////////////移植映射修改区////////////////////////////////////////////
static FIL *file = (FIL *) &SDFile;        //为file申请内存
////////////////////////////////////////////////////////////////////////////////////////


static HAL_StatusTypeDef fileDownload(const char *des_path) {
    uint32_t size = 0;
    HAL_StatusTypeDef state;
    uint8_t res;
    uint8_t temp[4]={0};
    HAL_UART_Receive(&huart1, (uint8_t *)&size, 1, 2000);
    state = HAL_UART_Receive(&huart1, (uint8_t *)&size, sizeof(uint32_t), 2000);
    if (state != HAL_OK) {
        return HAL_TIMEOUT;
    }
    if (size > 0) {
        memset(global_buff512, 0, 512);
        res = f_open(file, (const TCHAR *) des_path, FA_CREATE_ALWAYS | FA_WRITE);
        if (res)return HAL_ERROR;
        while (1) {
            if (size <= 512) {
                state = HAL_UART_Receive(&huart1, global_buff512, size, 2000);
                if (state == HAL_OK) {
                    if (f_write(file, global_buff512, size, NULL)) {
                        state = HAL_ERROR;
                        break;
                    }
                }
                break;
            } else {
                state = HAL_UART_Receive(&huart1, global_buff512, 512, 2000);
                if (state != HAL_OK) {
                    if (f_write(file, global_buff512, 512, NULL)) {
                        state = HAL_ERROR;
                        break;
                    }
                }
                size -= 512;
            }
            if (state != HAL_OK)break;
        }
        f_close(file);
    }
    return state;
}

HAL_StatusTypeDef downLoadFileList(File_TypeTypeDef type) {
    Printf("get_file_list:%d", type);
    HAL_StatusTypeDef state = HAL_OK;
    if (type == IMG) {
        state = fileDownload("img_list.txt");
    } else if (type == AUDIO) {
        state = fileDownload("audio_list.txt");
    }
    return state;
}

HAL_StatusTypeDef downLoadDitherPicBin(uint16_t file_id) {
    Printf("get_dither_bin:%d", file_id);
    char path_buff[50] = {0};
    snprintf(path_buff, 50, "img_%d.bin", file_id);
    return fileDownload(path_buff);
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

    if(file_ptr->fptr == 0){
        memset(global_buff512, 0, 512);
        sector_flag = last_sector_flag = buff_index = file_index = 0;
        if (f_read(file, global_buff512, sizeof(uint8_t) * 512, NULL))return FILE_INFO_ERROR;
    }

    while(1){
        if(last_sector_flag!=sector_flag){
            if(sector_flag==1){
                memset(global_buff512, 0, 256);
                if (f_read(file, global_buff512, sizeof(uint8_t) * 256, NULL))return FILE_INFO_ERROR;
            }
            else{
                memset(&global_buff512[256], 0, 256);
                if (f_read(file, &global_buff512[256], sizeof(uint8_t) * 256, NULL))return FILE_INFO_ERROR;
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

