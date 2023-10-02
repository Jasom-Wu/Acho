//
// Created by Mr.WU on 2023/9/30.
//

#include "page_main_select.h"
#include "broker_app.h"
#include "page_filelist.h"
#include "page_audio.h"
#include "page_play.h"
extern lv_indev_t *indev_encoder;

static void setup_screen(void *user_data);

static void resume_screen(void *user_data);

lv_ui ui_main_select = {
        .screen=NULL,
        .last_ui=NULL,
        .group=NULL,
        .is_init = false,
        .gui_setup_func = setup_screen,
        .gui_resume_func = resume_screen,
        .gui_delete_func = NULL
};

static void custom_anim_callback(void *obj, int32_t value) {
    char buff[50] = {0};
    lv_obj_t *target = obj;
    if (memcmp(target->user_data, "Audio", 6) == 0) {
        snprintf(buff, 50, "P:Static\\Animation\\main-selection\\music\\%ld.bin", value);
        lv_img_set_src(target, buff);
    } else if (memcmp(target->user_data, "Photo", 6) == 0) {
        snprintf(buff, 50, "P:Static\\Animation\\main-selection\\photo\\%ld.bin", value);
        lv_img_set_src(target, buff);
    }
}

static void animation_end_callback(struct _lv_anim_t *anim) {
    lv_obj_t * img_front,*img_pre=NULL;
    lv_obj_t * img_div = lv_obj_get_child(lv_obj_get_child(ui_main_select.screen,0),1);
    img_front = lv_obj_get_child(img_div,0);
    img_pre = lv_obj_get_child(img_div,1);
    lv_obj_set_style_img_opa(img_front, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_img_opa(img_pre, LV_OPA_0, LV_PART_MAIN);
}

static void click_event(lv_event_t *e) {
    lv_obj_t *label_div = (lv_obj_t *) lv_event_get_target(e);
    if (label_div) {
        lv_anim_t anim_mode_text;
        lv_anim_t anim_icon;
        char *mode_str = "";
        lv_obj_t *mode_text = lv_obj_get_child(label_div, 0);
        if (mode_text)
            mode_str = lv_label_get_text(mode_text);
        if (e->code == LV_EVENT_LONG_PRESSED) {
            delete_ui(&ui_main_select, NULL);

        } else if (e->code == LV_EVENT_SHORT_CLICKED) {
            if(ui_main_select.screen->user_data){
                if(memcmp(ui_main_select.screen->user_data,"PLAY",4)==0){
                    if (memcmp(mode_str, "Audio", 6) == 0) {
                        setup_ui(&ui_audio,&ui_main_select,NULL);
                    }else if (memcmp(mode_str, "Photo", 6) == 0){
                        setup_ui(&ui_play,&ui_main_select,"IMG");
                    }
                }
                else if(memcmp(ui_main_select.screen->user_data,"DOWNLOAD",8)==0){
                    if (memcmp(mode_str, "Audio", 6) == 0) {
                        setup_ui(&ui_filelist,&ui_main_select,"AUDIO");
                    }else if (memcmp(mode_str, "Photo", 6) == 0){
                        setup_ui(&ui_filelist,&ui_main_select,"IMG");
                    }
                }

            }

        } else {
            lv_anim_init(&anim_mode_text);
            lv_anim_set_var(&anim_mode_text, label_div);
            lv_anim_set_time(&anim_mode_text, 300);
            lv_anim_set_delay(&anim_mode_text, 0);
            lv_anim_set_exec_cb(&anim_mode_text, (lv_anim_exec_xcb_t) lv_obj_set_x);
            lv_obj_t * img_front,*img_pre=NULL;
            lv_obj_t * img_div = lv_obj_get_child(lv_obj_get_child(ui_main_select.screen,0),1);
            img_front = lv_obj_get_child(img_div,0);
            img_pre = lv_obj_get_child(img_div,1);
            if (e->code == LV_EVENT_FOCUSED && indev_encoder->proc.state!=LV_INDEV_STATE_PR) {//  emerge
                if (img_front) {
                    lv_anim_init(&anim_icon);
                    anim_icon.user_data = img_front;
                    img_front->user_data = mode_str;
                    lv_anim_set_var(&anim_icon, img_front);
                    lv_anim_set_exec_cb(&anim_icon, (lv_anim_exec_xcb_t) custom_anim_callback);
                    lv_anim_set_time(&anim_icon, 400);
                    lv_anim_set_delay(&anim_icon, 300);
                }
                if (memcmp(mode_str, "Audio", 6) == 0) {
                    lv_anim_set_values(&anim_mode_text, -200, 0);
                    lv_anim_set_values(&anim_icon, 1, 10);
                } else if (memcmp(mode_str, "Photo", 6) == 0) {
                    lv_anim_set_values(&anim_mode_text, -200, 0);
                    lv_anim_set_values(&anim_icon, 1, 15);
                }
                lv_anim_start(&anim_mode_text);
                lv_anim_start(&anim_icon);
            } else if (e->code == LV_EVENT_DEFOCUSED && indev_encoder->proc.state!=LV_INDEV_STATE_PR) {//  submerge
                if (img_pre) {
                    lv_obj_set_style_img_opa(img_front, LV_OPA_0, LV_PART_MAIN);
                    lv_obj_set_style_img_opa(img_pre, LV_OPA_100, LV_PART_MAIN);
                    lv_anim_init(&anim_icon);
                    anim_icon.user_data = img_pre;
                    img_pre->user_data = mode_str;
                    lv_anim_set_var(&anim_icon, img_pre);
                    lv_anim_set_exec_cb(&anim_icon, (lv_anim_exec_xcb_t) custom_anim_callback);
                    lv_anim_set_ready_cb(&anim_icon, animation_end_callback);
                    lv_anim_set_time(&anim_icon, 200);
                    lv_anim_set_delay(&anim_icon, 0);
                }
                if (memcmp(mode_str, "Audio", 6) == 0) {
                    lv_anim_set_values(&anim_mode_text, 0, 800);
                    lv_anim_set_values(&anim_icon, 11, 14);
                } else if (memcmp(mode_str, "Photo", 6) == 0) {
                    lv_anim_set_values(&anim_mode_text, 0, 200);
                    lv_anim_set_values(&anim_icon, 16, 18);
                }
                lv_anim_start(&anim_mode_text);
                lv_anim_start(&anim_icon);
            }
        }

    }


}

static void setup_screen(void *user_data) {
    ui_main_select.screen = lv_obj_create(NULL);
    lv_obj_set_user_data(ui_main_select.screen,user_data);
    lv_obj_set_scrollbar_mode(ui_main_select.screen,LV_SCROLLBAR_MODE_OFF);
    lv_obj_t *div = lv_obj_create(ui_main_select.screen);
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
    lv_label_set_text(title, "Select Mode");
    lv_obj_set_style_border_side(title, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(title, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(title, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN);


    lv_obj_t *img_div = lv_obj_create(div);
    lv_obj_set_size(img_div, LV_PCT(100), 90);
    lv_obj_set_style_pad_all(img_div, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(img_div, 2, LV_PART_MAIN);
    lv_obj_set_style_border_side(img_div, LV_BORDER_SIDE_TOP|LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_color(img_div, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(img_div,LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * img_front = lv_img_create(img_div);
    lv_obj_set_size(img_front, 100, 90);
    lv_obj_center(img_front);

    lv_obj_t * img_pre = lv_img_create(img_div);
    lv_obj_set_size(img_pre, 100, 90);
    lv_obj_center(img_pre);

    const char *mode_text[] = {"Audio", "Photo"};
    for (int i = 0; i < 2; i++) {
        lv_obj_t *label_div = lv_obj_create(ui_main_select.screen);
        lv_obj_set_size(label_div, LV_PCT(60), LV_SIZE_CONTENT);
        lv_obj_set_style_pad_all(label_div, 2, LV_PART_MAIN);
        lv_obj_t *label_mode = lv_label_create(label_div);
        lv_label_set_text(label_mode, mode_text[i]);
        lv_obj_center(label_mode);

        lv_obj_align(label_div, LV_ALIGN_BOTTOM_MID, 0, -20);
        lv_obj_add_style(label_div, &style_screen_list_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(label_div, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_add_style(label_div, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_x(label_div, -200);

        lv_obj_add_event_cb(label_div, click_event, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_event_cb(label_div, click_event, LV_EVENT_LONG_PRESSED, NULL);
        lv_obj_add_event_cb(label_div, click_event, LV_EVENT_FOCUSED, NULL);
        lv_obj_add_event_cb(label_div, click_event, LV_EVENT_DEFOCUSED, NULL);
        lv_group_add_obj(lv_group_get_default(), label_div);

    }


}

static void resume_screen(void *user_data) {
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
}