#include "gui_setup.h"

extern lv_indev_t *indev_encoder;
lv_obj_t *base_scr = NULL;
void ui_init_style(lv_style_t *style) {
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void setup_ui(lv_ui *ui, lv_ui *last_ui, void *user_data) {
    ui->last_ui = last_ui;
    if(ui->last_ui==NULL)
        base_scr = lv_scr_act();
    if (ui->group != NULL){
        lv_group_del(ui->group);
        ui->group = NULL;
        indev_encoder->group = NULL;
    }
    ui->group = lv_group_create();
    lv_group_set_default(ui->group);
    if (ui->gui_setup_func)
        ui->gui_setup_func(user_data);
    if(ui->group)
        lv_indev_set_group(indev_encoder, ui->group);

    ui->is_init = true;
    lv_scr_load(ui->screen);
}

void delete_ui(lv_ui *ui, void *user_data) {
    if (!ui->is_init)return;
    if (ui->gui_delete_func)
        ui->gui_delete_func(user_data);
    if(ui->group) {
        lv_group_del(ui->group);
        ui->group = NULL;
        indev_encoder->group = NULL; // internal bugs should add or hard fault.
    }
    lv_obj_del_async(ui->screen);
    ui->screen = NULL;
    ui->is_init = false;
    if (ui->last_ui == NULL) {
        lv_scr_load(base_scr);
    } else {
        lv_scr_load(ui->last_ui->screen);
        if(ui->last_ui->group)
            lv_indev_set_group(indev_encoder, ui->last_ui->group);
    }

}