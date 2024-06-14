//
// Created by Mr.WU on 2023/9/30.
//

#include "page_bind.h"
#include "broker_app.h"

extern lv_indev_t *indev_encoder;

static void setup_screen(void *user_data);

static void resume_screen(void *user_data);

lv_ui ui_bind = {
        .screen=NULL,
        .last_ui=NULL,
        .group=NULL,
        .is_init = false,
        .gui_setup_func = setup_screen,
        .gui_resume_func = resume_screen,
        .gui_delete_func = NULL
};

static void click_event(lv_event_t *e) {
    lv_obj_t *img = (lv_obj_t *) lv_event_get_target(e);
    if (img) {
        if (e->code == LV_EVENT_LONG_PRESSED) {
            delete_ui(&ui_bind, NULL);
        } else if (e->code == LV_EVENT_SHORT_CLICKED) {

        }
    }
}
static void TimerCallBack(lv_timer_t *timer){
    lv_obj_t *img = timer->user_data;
    if(img){
        if(img->user_data){
            lv_obj_t *hint = img->user_data;
            if(memcmp(hint->user_data,"0",1)==0){
                lv_img_set_src(img, "P:Setting\\QRCode_bind.bin");
            }
            else if(memcmp(hint->user_data,"1",1)==0){
                lv_obj_set_style_bg_opa(hint,LV_OPA_100,LV_PART_MAIN);
                lv_label_set_text(hint, LV_SYMBOL_CLOSE " TimeOut!");
                lv_timer_t *timer2 = lv_timer_create(TimerCallBack, 3000, img);
                lv_timer_set_repeat_count(timer2,1);
                hint->user_data = "2";
            } else if(memcmp(hint->user_data,"2",1)==0){
                delete_ui(&ui_bind,NULL);
            }
        }
    }
}
static void finishCallBackFunc(HAL_StatusTypeDef state,void *data){
    lv_obj_t *img = data;
    lv_obj_t *hint = NULL;
    if(img)
        hint = img->user_data;
    if(state==HAL_OK){
        hint->user_data = "0";
        lv_timer_t *timer = lv_timer_create(TimerCallBack, 0, img);
        lv_timer_set_repeat_count(timer,1);
    }
    else{
        hint->user_data = "1";
        lv_timer_t *timer = lv_timer_create(TimerCallBack, 0, img);
        lv_timer_set_repeat_count(timer,1);
    }

}
static void setup_screen(void *user_data) {
    ui_bind.screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(ui_bind.screen,LV_SCROLLBAR_MODE_OFF);
    lv_obj_t *div = lv_obj_create(ui_bind.screen);
    lv_obj_set_flex_flow(div, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(div, LV_PCT(94), LV_PCT(94));
    lv_obj_set_pos(div, LV_PCT(3), LV_PCT(3));
    lv_obj_set_style_radius(div, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(div, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(div, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(div, lv_color_make(0x00, 0x00, 0x00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(div, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(div, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(div, 5, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(div, LV_SCROLLBAR_MODE_OFF);
    resume_screen(NULL);
    lv_obj_t *title = lv_label_create(div);
    lv_obj_set_size(title, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(title, LV_PCT(0), LV_PCT(0));
    lv_label_set_text(title, "Scan to bind");
    lv_obj_set_style_border_side(title, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(title, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(title, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN);




    lv_obj_t *img_div = lv_obj_create(ui_bind.screen);
    lv_obj_set_size(img_div, 160, 160);
    lv_obj_set_style_pad_all(img_div, 0, LV_PART_MAIN);
    lv_obj_align(img_div,LV_ALIGN_CENTER,0,12);
    lv_obj_set_scrollbar_mode(img_div,LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * img = lv_img_create(img_div);
    lv_obj_set_size(img, 160, 160);
    lv_obj_center(img);

    lv_obj_t *hint = lv_label_create(ui_bind.screen);
    lv_obj_set_size(hint, LV_PCT(90), LV_SIZE_CONTENT);
    lv_obj_set_pos(hint, LV_PCT(5), LV_PCT(40));

    lv_obj_set_style_bg_opa(hint, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(hint, lv_color_make(0x00,0x00,0x00),LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(hint, lv_color_make(0xff,0xff,0xff),LV_PART_MAIN);
    lv_label_set_text(hint, "");
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_text_font(hint, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_user_data(img,hint);
    downLoadFile(QR,NULL,"QRCode_bind",finishCallBackFunc,img);

    lv_obj_add_event_cb(ui_bind.screen, click_event, LV_EVENT_LONG_PRESSED, NULL);
    lv_group_add_obj(lv_group_get_default(),ui_bind.screen);
    lv_group_focus_freeze(lv_group_get_default(),true);
}

static void resume_screen(void *user_data) {
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
}