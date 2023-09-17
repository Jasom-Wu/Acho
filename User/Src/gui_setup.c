//
// Created by Mr.WU on 2023/9/17.
//

#include "gui_setup.h"
lv_ui guider_ui;

void ui_init_style(lv_style_t * style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void setup_ui(lv_ui *ui)
{
    ui->last_screen = lv_scr_act();
    if(ui->gui_setup_func)
        ui->gui_setup_func();
    ui->is_init = true;
    lv_scr_load(ui->screen);
}

void delete_ui(lv_ui *ui){
    if(!ui->is_init)return;
    if(ui->gui_delete_func)
        ui->gui_delete_func();
    lv_obj_del_async(ui->screen);
    ui->screen = NULL;
    ui->is_init = false;
    lv_scr_load(ui->last_screen);
}
