//
// Created by Mr.WU on 2023/9/17.
//

#include "page_main.h"
#include "page_main_select.h"
#include <stdio.h>
#include "broker_app.h"
#include "sdio.h"

static void setup_screen(void *user_data);
static void resume_screen(void *user_data);
lv_ui ui_main = {
        .screen=NULL,
        .last_ui=NULL,
        .group=NULL,
        .is_init = false,
        .gui_setup_func = setup_screen,
        .gui_resume_func = resume_screen,
        .gui_delete_func = NULL
};


static void click_event(lv_event_t *e) {
    lv_label_t *target = (lv_label_t *) lv_event_get_target(e);
    if (e->code == LV_EVENT_SHORT_CLICKED) {
        if (memcmp(target->text, LV_SYMBOL_PLAY, 3) == 0) {
            setup_ui(&ui_main_select, &ui_main, "PLAY");
        } else if (memcmp(target->text, LV_SYMBOL_DOWNLOAD, 3) == 0) {
            setup_ui(&ui_main_select, &ui_main, "DOWNLOAD");
        } else if (memcmp(target->text, LV_SYMBOL_BELL, 3) == 0) {
            downLoadFileList(IMG,NULL,NULL);
        } else if (memcmp(target->text, LV_SYMBOL_USB, 3) == 0) {
            while(HAL_SD_GetState(&hsd)==HAL_SD_STATE_BUSY);
            HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_SET);
            HAL_Delay(50);
            HAL_GPIO_WritePin(USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_RESET);
        } else if (memcmp(target->text, "["LV_SYMBOL_CLOSE"]", 3) == 0) {
            delete_ui(&ui_main, NULL);
        }
    }
}

static void setup_screen(void *user_data) {
    ui_main.screen = lv_obj_create(NULL);
    lv_obj_t *screen_list = lv_obj_create(ui_main.screen);
    lv_obj_set_flex_flow(screen_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen_list, 15, LV_PART_MAIN);
    lv_obj_set_size(screen_list, 158, 150);
    lv_obj_set_pos(screen_list, 21, 20);
    lv_obj_set_style_radius(screen_list, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_list, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(screen_list, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_list, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(screen_list, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    resume_screen(NULL);
    const char *label_text[] = {LV_SYMBOL_PLAY " Play", LV_SYMBOL_DOWNLOAD "Download", LV_SYMBOL_BELL " GET-LIST",
                                LV_SYMBOL_USB "OPEN-USB"};
    for (int i = 0; i < 4; i++) {
        lv_obj_t *label = lv_label_create(screen_list);
        lv_label_set_text(label, label_text[i]);
        lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_add_style(label, &style_screen_list_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(label, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_add_style(label, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_add_event_cb(label, click_event, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
        lv_group_add_obj(lv_group_get_default(), label);
    }
    lv_obj_t *label = lv_label_create(ui_main.screen);
    lv_obj_set_pos(label, 150, 30);
    lv_label_set_text(label, "[" LV_SYMBOL_CLOSE"]");
    lv_obj_set_size(label, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(label, &style_screen_list_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(label, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(label, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_event_cb(label, click_event, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);

    lv_group_add_obj(lv_group_get_default(), label);
}

static void resume_screen(void *user_data){
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
}