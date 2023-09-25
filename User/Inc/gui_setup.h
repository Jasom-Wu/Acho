//
// Created by Mr.WU on 2023/9/17.
//

#ifndef INK_PAPER_GUI_SETUP_H
#define INK_PAPER_GUI_SETUP_H


#include "lvgl.h"


typedef struct lv_ui
{
    lv_obj_t *screen;
    struct lv_ui *last_ui;
    lv_group_t *group;
    uint8_t is_init;
    void (*gui_setup_func)(void * user_data);
    void (*gui_delete_func)(void * user_data);
}lv_ui;

void ui_init_style(lv_style_t * style);
void setup_ui(lv_ui *ui,lv_ui *last_ui,void *user_data);
void delete_ui(lv_ui *ui,void *user_data);

#endif //INK_PAPER_GUI_SETUP_H
