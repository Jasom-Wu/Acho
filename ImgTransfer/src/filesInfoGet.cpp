#include "filesInfoGet.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

/*
 * 函数：获取文件夹dirpath下的所有文件
 * 返回：
 *      File_Typedef 返回一个描述结构体链表，元素包含文件全名、类型、大小。
 *      counts 通过指针返回读取到的文件个数。
 */
File_Typedef * getImgsFileInfo(const char* dirpath,long *counts)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    LARGE_INTEGER filesize;
    long files_counts=0;
    long mem_counts = 10;

    File_Typedef *files_list = (File_Typedef*)malloc(sizeof(File_Typedef)*mem_counts);
    memset(files_list,0,sizeof(File_Typedef)*mem_counts);
    File_Typedef *file = NULL;
    char *temp = (char*)malloc(strlen(dirpath)+4);
    memset(temp,0,strlen(dirpath)+4);
    sprintf(temp,"%s\\*",dirpath);
    char *temp_type;
    //printf("%d",strlen(dirpath));
    //printf("%s",temp);
    //printf("%d",strlen(temp));
    hFind = FindFirstFile(temp, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        printf ("\nFindFirstFile failed (%lu)\n", GetLastError());
        return 0;
    }
    do
    {
        if(FindFileData.cFileName[0] == '.')//过滤.和..
                continue;
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);
        }
        else
        {
            filesize.LowPart = FindFileData.nFileSizeLow;
            filesize.HighPart = FindFileData.nFileSizeHigh;
            //printf(TEXT("  %s   %ld bytes\n"), FindFileData.cFileName, filesize.QuadPart);
            if(files_counts>=mem_counts)
            {
                files_list = (File_Typedef*)realloc(files_list,(mem_counts+10)*sizeof(File_Typedef));//扩容
                mem_counts+=10;
            }
            else
            {
                file = &files_list[files_counts];
                if(strlen(FindFileData.cFileName)<NAME_MAX_SIZE)
                    strcpy(file->file_name,FindFileData.cFileName);
                else
                    strcpy(file->file_name,"FILE NAME TOO LONG!");

                temp_type = strchr(file->file_name,'.');
                if(strlen(temp_type+=1)<TYPE_MAX_SIZE)
                    strcpy(file->type,temp_type);
                else
                    strcpy(file->type,"2 long!");
                file->size = filesize.QuadPart;
                files_counts++;
            }
        }
    }
    while (FindNextFile(hFind, &FindFileData) != 0);
    FindClose(hFind);
    free(temp);
    files_list = (File_Typedef *)realloc(files_list,(files_counts)*sizeof(File_Typedef));
    if (counts !=NULL)
        *counts = files_counts;
    else
        counts =NULL;
    return files_list;
}
