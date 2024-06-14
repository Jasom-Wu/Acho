#ifndef _FILES_INFO_GET_H_
#define _FILES_INFO_GET_H_
//#pragma

#define NAME_MAX_SIZE 20
#define TYPE_MAX_SIZE 8
typedef struct FILE_TYPE
{
    char file_name[NAME_MAX_SIZE];
    char type[TYPE_MAX_SIZE];
    long long size;
}File_Typedef;
#if NAME_MAX_SIZE<20 
#error  NAME_MAX_SIZE too small! at least 20.
#endif
File_Typedef * getImgsFileInfo(const char* dirpath,long *counts);
#endif