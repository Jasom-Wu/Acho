//
// Created by Mr.WU on 2023/9/17.
//

#ifndef INK_PAPER_GUI_SETUP_H
#define INK_PAPER_GUI_SETUP_H


#include "lvgl.h"


typedef struct
{
    lv_obj_t *screen;
    lv_obj_t *last_screen;
    uint8_t is_init;
    void (*gui_setup_func)(void);
    void (*gui_delete_func)(void);
}lv_ui;

void ui_init_style(lv_style_t * style);
void setup_ui(lv_ui *ui);
void delete_ui(lv_ui *ui);
#endif //INK_PAPER_GUI_SETUP_H
