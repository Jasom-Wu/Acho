//
// Created by Mr.WU on 2023/9/17.
//

#include "page_test.h"
#include <stdio.h>
#include "broker_app.h"


static void setup_scr_screen(void);

lv_ui ui_test = {
    .screen=NULL,
    .last_screen=NULL,
    .is_init = false,
    .gui_setup_func = setup_scr_screen,
    .gui_delete_func = NULL
};


static void click_event(lv_event_t * e)
{
    lv_label_t * target = (lv_label_t *)lv_event_get_target(e);
    if(memcmp(target->text,LV_SYMBOL_PLAY,3)==0){

    }
    else if(memcmp(target->text,LV_SYMBOL_DRIVE,3)==0){
        downLoadDitherPicBin(2);
    }
    else if(memcmp(target->text,LV_SYMBOL_BELL,3)==0){
        downLoadFileList(IMG);
    }
}

static void setup_scr_screen(void){
    //Write codes screen
    ui_test.screen = lv_obj_create(NULL);
    lv_obj_t * screen_list = lv_obj_create(ui_test.screen);
    lv_obj_set_flex_flow(screen_list,LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen_list,15,LV_PART_MAIN);
    lv_obj_set_pos(screen_list, 20, 5);
    lv_obj_set_size(screen_list, 158, 150);
    lv_obj_set_style_radius(screen_list, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(screen_list, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(screen_list, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(screen_list, 255, LV_PART_MAIN|LV_STATE_DEFAULT);


    //Set style state: LV_STATE_DEFAULT for style_screen_list_extra_btns_main_default
    static lv_style_t style_screen_list_extra_btns_main_default;
    ui_init_style(&style_screen_list_extra_btns_main_default);
    lv_style_set_radius(&style_screen_list_extra_btns_main_default, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_default, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);

    //Set style state: LV_STATE_PRESSED for style_screen_list_extra_btns_main_pressed
    static lv_style_t style_screen_list_extra_btns_main_pressed;
    ui_init_style(&style_screen_list_extra_btns_main_pressed);
    lv_style_set_radius(&style_screen_list_extra_btns_main_pressed, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_pressed, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_pressed, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_pressed, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);

    //Set style state: LV_STATE_FOCUSED for style_screen_list_extra_btns_main_focused
    static lv_style_t style_screen_list_extra_btns_main_focused;
    ui_init_style(&style_screen_list_extra_btns_main_focused);
    lv_style_set_radius(&style_screen_list_extra_btns_main_focused, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_focused, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_focused, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_focused, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
    lv_style_set_text_align(&style_screen_list_extra_btns_main_focused,LV_TEXT_ALIGN_CENTER);
    lv_style_set_outline_color(&style_screen_list_extra_btns_main_focused,lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_outline_opa(&style_screen_list_extra_btns_main_focused,255);
    lv_style_set_outline_width(&style_screen_list_extra_btns_main_focused,1);
    lv_style_set_outline_pad(&style_screen_list_extra_btns_main_focused,2);

    lv_group_t * g = lv_group_create();

    const char* label_text[]={LV_SYMBOL_PLAY " Play",LV_SYMBOL_DRIVE "Download",LV_SYMBOL_BELL " Get-list"};
    for(int i=0;i<3;i++)
    {
        lv_obj_t * label = lv_label_create(screen_list);
        lv_label_set_text(label, label_text[i]);
        lv_obj_set_size(label, LV_PCT(100), LV_SIZE_CONTENT);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_add_style(label, &style_screen_list_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

        lv_obj_add_style(label, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN|LV_STATE_PRESSED);
        lv_obj_add_event_cb(label,click_event,LV_EVENT_CLICKED,NULL);
        lv_obj_add_style(label, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

        lv_group_add_obj(g,label);
    }
    extern lv_indev_t * indev_encoder;
    lv_indev_set_group(indev_encoder,g);
}