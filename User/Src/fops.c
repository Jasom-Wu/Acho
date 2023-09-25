//////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @file  fops.c
 * @autor lakun@qq.com
 * @data  2020/3/5
 * @note  FatFs操作SD卡API
 */
//////////////////////////////////////////////////////////////////////////////////////////////

#include "fops.h"
#include "ff.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"


FILINFO fileinfo;    //文件信息
#if _USE_LFN
char namebuff[_MAX_LFN * 2 + 1];
#endif
const uint8_t *FILE_TYPE_TBL[6][13] =
        {
                {"BIN"},            //BIN文件
                {"LRC"},            //LRC文件
                {"NES"},            //NES文件
                {"TXT", "C",   "H"},    //文本文件
                {"MP1", "MP2", "MP3",  "MP4", "M4A", "3GP", "3G2", "OGG", "ACC", "WMA", "WAV", "MID", "FLAC"},//音乐文件
                {"BMP", "JPG", "JPEG", "GIF"},//图片文件
        };


//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
uint32_t exf_showfree(uint8_t *drv) {
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR *) drv, (DWORD *) &fre_clust, &fs1);
    if (res == 0) {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
        fre_sect = fre_clust * fs1->csize;            //得到空闲扇区数
#if _MAX_SS != 512
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        if (tot_sect < 20480)//总容量小于10M
        {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n磁盘总容量:%d KB\r\n"
                   "可用空间:%d KB\r\n",
                   tot_sect >> 1, fre_sect >> 1);
        } else {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n磁盘总容量:%d MB\r\n"
                   "可用空间:%d MB\r\n",
                   tot_sect >> 11, fre_sect >> 11);
        }
    }
    return fre_sect;
}

void exf_mount(void) {
    printf("# SD Card Mount %s!\r\n", f_mount(&SDFatFS, SDPath, 1) == FR_OK ? "Successfullly" : "Failed");
}


uint8_t exf_readdir(DIR *dir) {
    uint8_t res;
    char *fn;
#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = namebuff;
#endif
    res = f_readdir(dir, &fileinfo);//读取一个文件的信息
    if (res != FR_OK || fileinfo.fname[0] == 0) {
        printf("error");
        return res;//读完了.
    }
#if _USE_LFN
    fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
    fn=fileinfo.fname;
#endif
    printf("\r\n DIR info:\r\n");

    printf("dir.id:%d\r\n", dir->id);
    printf("dir.index:%d\r\n", dir->index);
    printf("dir.sclust:%d\r\n", dir->sclust);
    printf("dir.clust:%d\r\n", dir->clust);
    printf("dir.sect:%d\r\n", dir->sect);

    printf("\r\n");
    printf("File Name is:%s\r\n", fn);
    printf("File Size is:%d\r\n", fileinfo.fsize);
    printf("File data is:%d\r\n", fileinfo.fdate);
    printf("File time is:%d\r\n", fileinfo.ftime);
    printf("File Attr is:%d\r\n", fileinfo.fattrib);
    printf("\r\n");
    //myfree(fileinfo.lfname);
    return 0;
}

//遍历文件
//path:路径
//返回值:执行结果
uint8_t exf_scan_files(DIR *dir, char **file_name,DWORD *file_size) {
    if (!dir || !file_name)return FR_INVALID_PARAMETER;
    FRESULT res;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    memset(namebuff,0, sizeof(namebuff));
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = namebuff;//mymalloc(fileinfo.lfsize);
#endif
    if (file_name != NULL)
        *file_name = NULL;
//    res = f_opendir(dir, (const TCHAR *) path); //打开一个目录
    while (1) {
        res = f_readdir(dir, &fileinfo);                   //读取目录下的一个文件
        if (res != FR_OK || fileinfo.fname[0] == 0) {
            f_closedir(dir);
            return FR_NO_FILE;
        }  //错误了/到末尾了,退出
        if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
        fn = fileinfo.fname;
#endif                                                  /* It is a file. */
        *file_name = fn;
        if(file_size)
            *file_size = fileinfo.fsize;
        break;
    }

//	myfree(fileinfo.lfname);
    return res;
}

//将小写字母转为大写字母,如果是数字,则保持不变.
uint8_t char_upper(uint8_t c) {
    if (c < 'A')return c;//数字,保持不变.
    if (c >= 'a')return c - 0x20;//变为大写.
    else return c;//大写,保持不变
}

//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
uint8_t f_typetell(uint8_t *fname) {
    uint8_t tbuf[5];
    uint8_t *attr = '\0';//后缀名
    uint8_t i = 0, j;
    while (i < 250) {
        i++;
        if (*fname == '\0')break;//偏移到了最后了.
        fname++;
    }
    if (i == 250)return 0XFF;//错误的字符串.
    for (i = 0; i < 5; i++)//得到后缀名
    {
        fname--;
        if (*fname == '.') {
            fname++;
            attr = fname;
            break;
        }
    }
    strcpy((char *) tbuf, (const char *) attr);//copy
    for (i = 0; i < 4; i++)tbuf[i] = char_upper(tbuf[i]);//全部变为大写
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 13; j++) {
            if (*FILE_TYPE_TBL[i][j] == 0)break;//此组已经没有可对比的成员了.
            if (strcmp((const char *) FILE_TYPE_TBL[i][j], (const char *) tbuf) == 0)//找到了
            {
                return (i << 4) | j;
            }
        }
    }
    return 0XFF;//没找到
}

#if _USE_LABEL
//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void exf_getlabel(uint8_t *path)
{
    uint8_t buf[20];
    uint32_t sn=0;
    uint8_t res;
    res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
    if(res==FR_OK)
    {
        printf("\r\n磁盘%s 的盘符为:%s\r\n",path,buf);
        printf("磁盘%s 的序列号:%X\r\n\r\n",path,sn);
    }else printf("\r\n获取失败，错误码:%X\r\n",res);
}
//设置盘符（磁盘名字），最长11个字符！！，支持数字和大写字母组合以及汉字等
//path:磁盘号+名字，比如"0:ALIENTEK"、"1:OPENEDV"
void exf_setlabel(uint8_t *path)
{
    uint8_t res;
    res=f_setlabel ((const TCHAR *)path);
    if(res==FR_OK)
    {
        printf("\r\n磁盘盘符设置成功:%s\r\n",path);
    }else printf("\r\n磁盘盘符设置失败，错误码:%X\r\n",res);
}
#endif
