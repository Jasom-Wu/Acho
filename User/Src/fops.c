//////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @file  fops.c
 * @autor lakun@qq.com
 * @data  2020/3/5
 * @note  FatFs����SD��API
 */
//////////////////////////////////////////////////////////////////////////////////////////////

#include "fops.h"
#include "ff.h"
#include "fatfs.h"
#include "stdio.h"
#include "string.h"


FILINFO fileinfo;    //�ļ���Ϣ
#if _USE_LFN
char namebuff[_MAX_LFN * 2 + 1];
#endif
const uint8_t *FILE_TYPE_TBL[6][13] =
        {
                {"BIN"},            //BIN�ļ�
                {"LRC"},            //LRC�ļ�
                {"NES"},            //NES�ļ�
                {"TXT", "C",   "H"},    //�ı��ļ�
                {"MP1", "MP2", "MP3",  "MP4", "M4A", "3GP", "3G2", "OGG", "ACC", "WMA", "WAV", "MID", "FLAC"},//�����ļ�
                {"BMP", "JPG", "JPEG", "GIF"},//ͼƬ�ļ�
        };


//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
uint32_t exf_showfree(uint8_t *drv) {
    FATFS *fs1;
    uint8_t res;
    uint32_t fre_clust = 0, fre_sect = 0, tot_sect = 0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR *) drv, (DWORD *) &fre_clust, &fs1);
    if (res == 0) {
        tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
        fre_sect = fre_clust * fs1->csize;            //�õ�����������
#if _MAX_SS != 512
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        if (tot_sect < 20480)//������С��10M
        {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n����������:%d KB\r\n"
                   "���ÿռ�:%d KB\r\n",
                   tot_sect >> 1, fre_sect >> 1);
        } else {
            /* Print free space in unit of KB (assuming 512 bytes/sector) */
            printf("\r\n����������:%d MB\r\n"
                   "���ÿռ�:%d MB\r\n",
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
    res = f_readdir(dir, &fileinfo);//��ȡһ���ļ�����Ϣ
    if (res != FR_OK || fileinfo.fname[0] == 0) {
        printf("error");
        return res;//������.
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

//�����ļ�
//path:·��
//����ֵ:ִ�н��
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
//    res = f_opendir(dir, (const TCHAR *) path); //��һ��Ŀ¼
    while (1) {
        res = f_readdir(dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
        if (res != FR_OK || fileinfo.fname[0] == 0) {
            f_closedir(dir);
            return FR_NO_FILE;
        }  //������/��ĩβ��,�˳�
        if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
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

//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
uint8_t char_upper(uint8_t c) {
    if (c < 'A')return c;//����,���ֲ���.
    if (c >= 'a')return c - 0x20;//��Ϊ��д.
    else return c;//��д,���ֲ���
}

//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//		 ����,����λ��ʾ��������,����λ��ʾ����С��.
uint8_t f_typetell(uint8_t *fname) {
    uint8_t tbuf[5];
    uint8_t *attr = '\0';//��׺��
    uint8_t i = 0, j;
    while (i < 250) {
        i++;
        if (*fname == '\0')break;//ƫ�Ƶ��������.
        fname++;
    }
    if (i == 250)return 0XFF;//������ַ���.
    for (i = 0; i < 5; i++)//�õ���׺��
    {
        fname--;
        if (*fname == '.') {
            fname++;
            attr = fname;
            break;
        }
    }
    strcpy((char *) tbuf, (const char *) attr);//copy
    for (i = 0; i < 4; i++)tbuf[i] = char_upper(tbuf[i]);//ȫ����Ϊ��д
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 13; j++) {
            if (*FILE_TYPE_TBL[i][j] == 0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
            if (strcmp((const char *) FILE_TYPE_TBL[i][j], (const char *) tbuf) == 0)//�ҵ���
            {
                return (i << 4) | j;
            }
        }
    }
    return 0XFF;//û�ҵ�
}

#if _USE_LABEL
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"  
void exf_getlabel(uint8_t *path)
{
    uint8_t buf[20];
    uint32_t sn=0;
    uint8_t res;
    res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
    if(res==FR_OK)
    {
        printf("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
        printf("����%s �����к�:%X\r\n\r\n",path,sn);
    }else printf("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"
void exf_setlabel(uint8_t *path)
{
    uint8_t res;
    res=f_setlabel ((const TCHAR *)path);
    if(res==FR_OK)
    {
        printf("\r\n�����̷����óɹ�:%s\r\n",path);
    }else printf("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
}
#endif
