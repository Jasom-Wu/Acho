//
// Created by Mr.WU on 2023/9/23.
//
#include "page_play.h"
#include "fops.h"
#include "broker_app.h"
#include "EPD_1in54_V2.h"
#include "page_audio.h"
#include "sdio.h"

static void setup_screen(void *user_data);

static void resume_screen(void *user_data);
lv_ui ui_play = {
        .screen=NULL,
        .last_ui=NULL,
        .is_init = false,
        .gui_setup_func = setup_screen,
        .gui_resume_func = resume_screen,
        .gui_delete_func =NULL,
};


static void imgPlay_event(lv_event_t *e){
    lv_obj_t *target = (lv_obj_t *)lv_event_get_target(e);
    if (e->code == LV_EVENT_LONG_PRESSED) {
        lv_group_focus_freeze(lv_group_get_default(),false);
        if(target->user_data){
            lv_group_focus_obj(target->user_data);
        }
        lv_group_remove_obj(target);
        lv_obj_del_async(target);
        EPD_1N54_V2_FullClearToPartial();
    }
}
static void click_event(lv_event_t *e) {
    char *name;
    lv_obj_t *target = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *label_name = lv_obj_get_child(target,0);
    if (e->code == LV_EVENT_LONG_PRESSED) {
        delete_ui(&ui_play, NULL);
    } else if(label_name){
        if (e->code == LV_EVENT_SHORT_CLICKED) {
            name = lv_label_get_text(label_name);
            if (name) {
                char path[50] = {0};
                char *type = lv_obj_get_user_data(ui_play.screen);
                if (memcmp(type, "IMG", 3) == 0) {
                    snprintf(path,50,"P:Images\\%s",name);
                    lv_obj_t *div = lv_obj_create(ui_play.screen);
                    lv_obj_add_flag(div,LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_set_size(div,LV_PCT(100),LV_PCT(100));
                    lv_obj_set_style_border_width(div,5,LV_PART_MAIN);
                    lv_obj_set_style_border_color(div, lv_color_make(0,0,0),LV_PART_MAIN);
                    lv_obj_set_user_data(div,target);
                    lv_obj_add_event_cb(div,imgPlay_event,LV_EVENT_LONG_PRESSED,div);
                    lv_group_add_obj(lv_group_get_default(),div);
                    lv_group_focus_obj(div);
                    lv_group_focus_freeze(lv_group_get_default(),true);
                    lv_obj_t *img = lv_img_create(div);
                    lv_img_set_src(img, path);
                    lv_obj_center(img);
                } else if (memcmp(type, "AUDIO", 5) == 0){
                    snprintf(path,50,"P:Audios\\%s",name);
                    if(target->user_data && ui_audio.user_data==NULL){
                        ui_audio.user_data = lv_mem_alloc(2);
                        memcpy(ui_audio.user_data,target->user_data,2);
                        delete_ui(&ui_play, NULL);
                    }
                }
            }
        } else if (e->code == LV_EVENT_FOCUSED) {
            lv_obj_scroll_to_view_recursive(target, LV_ANIM_OFF);
            lv_label_set_long_mode(label_name, LV_LABEL_LONG_SCROLL_CIRCULAR); // only 'label' obj can use this function!
        } else if (e->code == LV_EVENT_DEFOCUSED) {
            lv_label_set_long_mode(label_name, LV_LABEL_LONG_DOT);
        }
    }

}

static void setup_screen(void *user_data) {
    ui_play.screen = lv_obj_create(NULL);
    lv_obj_set_user_data(ui_play.screen, user_data);
    lv_obj_t *div = lv_obj_create(ui_play.screen);
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


    lv_obj_t *title = lv_label_create(div);
    lv_obj_set_size(title, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_pos(title, LV_PCT(0), LV_PCT(0));
    lv_label_set_text(title, "Select A File");
    lv_obj_set_style_border_side(title, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(title, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_t *screen_list = lv_obj_create(div);
    lv_obj_set_flex_flow(screen_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen_list, 5, LV_PART_MAIN);
    lv_obj_set_style_pad_left(screen_list, 10, LV_PART_MAIN);
    lv_obj_set_size(screen_list, LV_PCT(100), LV_PCT(82));
    lv_obj_set_pos(screen_list, LV_PCT(0), LV_PCT(0));
    lv_obj_set_style_bg_color(screen_list, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_list, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(screen_list, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(screen_list, LV_DIR_VER);
    lv_obj_set_style_bg_color(screen_list, lv_color_make(0x00, 0x00, 0x00), LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_opa(screen_list, LV_OPA_100, LV_PART_SCROLLBAR);

    resume_screen(NULL);
    FRESULT state = FR_INT_ERR;
    bool is_audio = false;
    DIR *dir = &SDDir;
    if (memcmp(user_data, "IMG", 3) == 0){
        while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
        state = f_opendir(dir, (const TCHAR *) "Images");
    }
    else if (memcmp(user_data, "AUDIO", 5) == 0){
        HeapManager.init(20);
        is_audio = true;
        while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
        state = f_opendir(dir, (const TCHAR *) "Audios");
    }
    char *file_name;
    DWORD file_size;
    uint16_t file_count = 0;
    if (state == FR_OK) {
        while (true) {
            while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
            state = exf_scan_files(dir, &file_name, &file_size,0);
            if (state == FR_OK) {
                lv_obj_t *file_div = lv_obj_create(screen_list);
                lv_obj_set_size(file_div, LV_PCT(95), LV_SIZE_CONTENT);
                lv_obj_set_flex_flow(file_div,LV_FLEX_FLOW_ROW);
                lv_obj_add_style(file_div, &style_screen_list_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_add_style(file_div, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
                lv_obj_add_style(file_div, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN | LV_STATE_FOCUSED);
                lv_obj_add_flag(file_div, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_add_event_cb(file_div, click_event, LV_EVENT_SHORT_CLICKED, NULL);
                lv_obj_add_event_cb(file_div, click_event, LV_EVENT_LONG_PRESSED, NULL);
                lv_obj_add_event_cb(file_div, click_event, LV_EVENT_FOCUSED, NULL);
                lv_obj_add_event_cb(file_div, click_event, LV_EVENT_DEFOCUSED, NULL);
                lv_obj_set_style_pad_all(file_div,0,LV_PART_MAIN);


                lv_obj_t *label_name = lv_label_create(file_div);
                lv_obj_t *label_size = lv_label_create(file_div);
                lv_obj_set_flex_grow(label_name,5);
                lv_obj_set_flex_grow(label_size,4);
                lv_obj_set_height(label_name,LV_SIZE_CONTENT);
                lv_obj_set_height(label_size,LV_SIZE_CONTENT);

                char size_buff[10]={0};
                if (file_size >= 0 && file_size < 1024) {
                    snprintf(size_buff, 10, "%ldB", file_size);
                } else if (file_size >= 1024 && file_size < 1048576) {
                    snprintf(size_buff, 10, "%.2fKB", (double) file_size / 1024);
                } else {
                    snprintf(size_buff, 10, "%.2fMB", (double) file_size / 1048576);
                }
                lv_label_set_text(label_name, file_name);
                lv_label_set_text(label_size, size_buff);
                lv_label_set_long_mode(label_name,LV_LABEL_LONG_DOT);
                lv_label_set_long_mode(label_size,LV_LABEL_LONG_DOT);
                if(is_audio)
                    lv_obj_set_user_data(file_div, HeapManager.push(&file_count,2));
                lv_group_add_obj(lv_group_get_default(), file_div);
                file_count++;
            } else {
                break;
            }
        }
    }
    f_closedir(dir);
    if (file_count == 0) {
        lv_obj_add_flag(div, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(div, click_event, LV_EVENT_LONG_PRESSED, NULL);
        lv_group_add_obj(lv_group_get_default(), div);
    }
}

static void resume_screen(void *user_data){
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_14);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_14);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_14);
}

static void delete_screen(void *user_data) {
    HeapManager.reset();
}
