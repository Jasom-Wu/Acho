
#ifndef __FOPS_H
#define __FOPS_H

#include "integer.h"
#include "main.h"
#include "ff.h"

uint32_t exf_showfree(uint8_t *drv);
void exf_mount(void);
uint8_t exf_scan_files(DIR *dir, char **file_name,DWORD *file_size);
uint8_t exf_readdir(DIR *dir);
uint8_t char_upper(uint8_t c);
uint8_t f_typetell(uint8_t *fname);

#if _USE_LABEL
void exf_getlabel(uint8_t *path);
void exf_setlabel(uint8_t *path);
#endif
#endif
