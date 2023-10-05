//
// Created by Mr.WU on 2023/10/2.
//
#include <stdio.h>
#include "page_audio.h"
#include "fatfs.h"
#include "page_play.h"
#include "fops.h"
#include "audio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sdio.h"


extern uint8_t audio_play_current_state;

static void setup_screen(void *user_data);

static void resume_screen(void *user_data);

static void delete_screen(void *user_data);

static uint16_t file_index = 0;
static uint16_t max_file_count = 0;
uint8_t audio_play_state = 0;
lv_ui ui_audio = {
        .screen=NULL,
        .last_ui=NULL,
        .group=NULL,
        .is_init = false,
        .gui_setup_func = setup_screen,
        .gui_resume_func = resume_screen,
        .gui_delete_func = delete_screen
};


static void custom_anim_callback(void *obj, int32_t value) {
    char buff[60] = {0};
    lv_obj_t *target = obj;
    if (target->user_data) {
        if (memcmp(target->user_data, "W", 1) == 0 || memcmp(target->user_data, "WE", 2) == 0) {
            if (((audio_play_current_state == AUDIO_HALT && audio_play_state == AUDIO_RESUME) ||
                 (audio_play_current_state == AUDIO_NONE && audio_play_state == AUDIO_PRE_PLAY)) && value > 8) {
                audio_play_state = AUDIO_PLAY;
                target->user_data = NULL;
            } else {
                snprintf(buff, 60, "P:Static\\Animation\\audio\\work\\%ld.bin", value);
                lv_img_set_src(target, buff);
            }
        } else if (memcmp(target->user_data, "D-O", 3) == 0) {
            if (value <= 9) {
                snprintf(buff, 60, "P:Static\\Animation\\audio\\drop\\%ld.bin", value);
                lv_img_set_src(target, buff);
            } else {
                snprintf(buff, 60, "P:Static\\Animation\\audio\\open\\%ld.bin", value - 8);
                lv_img_set_src(target, buff);
            }
        } else if (memcmp(target->user_data, "O-W", 3) == 0) {
            if (value <= 7) {
                snprintf(buff, 60, "P:Static\\Animation\\audio\\open\\%ld.bin", value);
                lv_img_set_src(target, buff);
            }
        } else if (memcmp(target->user_data, "O", 1) == 0) {
            snprintf(buff, 60, "P:Static\\Animation\\audio\\open\\%ld.bin", value);
            lv_img_set_src(target, buff);
        } else if (memcmp(target->user_data, "C", 1) == 0) {
            snprintf(buff, 60, "P:Static\\Animation\\audio\\close\\%ld.bin", value);
            lv_img_set_src(target, buff);
        } else if (memcmp(target->user_data, "D", 1) == 0) {
            snprintf(buff, 60, "P:Static\\Animation\\audio\\drop\\%ld.bin", value);
            lv_img_set_src(target, buff);
        }
    }
}

static void animation_start_callback(struct _lv_anim_t *anim) {
    lv_obj_t *img = anim->user_data;
    lv_obj_t *img_front, *img_pre = NULL;
    lv_obj_t *img_div = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 1);
    img_front = lv_obj_get_child(img_div, 0);
    img_pre = lv_obj_get_child(img_div, 1);
    if (memcmp(img->user_data, "W", 1) == 0 || memcmp(img->user_data, "O-W", 3) == 0) {
        lv_obj_set_style_img_opa(img_front, LV_OPA_0, LV_PART_MAIN);
        lv_obj_set_style_img_opa(img_pre, LV_OPA_100, LV_PART_MAIN);
    } else if (memcmp(img->user_data, "D", 1) == 0 || memcmp(img->user_data, "D-O", 3) == 0 ||
               memcmp(img->user_data, "C", 1) == 0) {
        lv_obj_set_style_img_opa(img_front, LV_OPA_100, LV_PART_MAIN);
        lv_obj_set_style_img_opa(img_pre, LV_OPA_0, LV_PART_MAIN);
    }
}

static void animation_end_callback(struct _lv_anim_t *anim) {
    lv_obj_t *img = anim->user_data;
    lv_obj_t *img_front, *img_pre = NULL;
    lv_obj_t *img_div = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 1);
    img_front = lv_obj_get_child(img_div, 0);
    img_pre = lv_obj_get_child(img_div, 1);
    if (img) {
        if (img->user_data) {
            if (memcmp(img->user_data, "W", 1) == 0) {
                lv_anim_set_time(anim, 3000);
                lv_anim_set_delay(anim, 200);
                lv_anim_set_values(anim, 1, 9);
                lv_anim_start(anim);
            } else if (memcmp(img->user_data, "O-W", 3) == 0) {
                img_pre->user_data = "W";
                lv_anim_set_time(anim, 1000);
                lv_anim_set_delay(anim, 200);
                lv_anim_set_values(anim, 1, 9);
                lv_anim_start(anim);
            }
        }
    }
}

static void startAnim(lv_obj_t *tar, char *mode, uint32_t duration, uint32_t delay, int32_t start, int32_t end) {
    lv_anim_del(tar, custom_anim_callback);
    lv_anim_t anim_box;
    lv_anim_init(&anim_box);
    anim_box.user_data = tar;
    tar->user_data = mode;
    lv_anim_set_start_cb(&anim_box, animation_start_callback);
    lv_anim_set_ready_cb(&anim_box, animation_end_callback);
    lv_anim_set_exec_cb(&anim_box, (lv_anim_exec_xcb_t) custom_anim_callback);
    lv_anim_set_var(&anim_box, tar);
    lv_anim_set_time(&anim_box, duration);
    lv_anim_set_delay(&anim_box, delay);
    lv_anim_set_values(&anim_box, start, end);
    lv_anim_start(&anim_box);
}

static void click_event(lv_event_t *e) {
    lv_obj_t *btn = (lv_obj_t *) lv_event_get_target(e);
    if (btn) {
        if (e->code == LV_EVENT_LONG_PRESSED) {
            audio_play_state = AUDIO_CANCEL;
            while (audio_play_current_state != AUDIO_NONE);
            delete_ui(&ui_audio, NULL);

        } else if (e->code == LV_EVENT_SHORT_CLICKED) {
            lv_obj_t *img_front, *img_pre = NULL;
            lv_obj_t *img_div = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 1);
            img_front = lv_obj_get_child(img_div, 0);
            img_pre = lv_obj_get_child(img_div, 1);
            lv_obj_t *title = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 0);
            if (memcmp(lv_label_get_text(btn), LV_SYMBOL_PAUSE, 3) == 0) {
                lv_label_set_text(btn, LV_SYMBOL_PLAY);
                audio_play_state = AUDIO_HALT;
                while (audio_play_current_state != AUDIO_HALT) {
                    vTaskDelay(100);
                }
                img_pre->user_data = "WE";
                startAnim(img_front, "C", 500, 500, 1, 7);
            } else if (memcmp(lv_label_get_text(btn), LV_SYMBOL_PLAY, 3) == 0) {
                lv_label_set_text(btn, LV_SYMBOL_PAUSE);
                DIR *dir = &SDDir;
                char *file_name;
                FRESULT state;
                if (audio_play_state == AUDIO_NONE) {
                    f_opendir(dir, (const TCHAR *) "Audios");
                    state = exf_scan_files(dir, &file_name, NULL, file_index);
                    f_closedir(dir);
                    if (state == FR_OK) {
                        lv_label_set_text(title, file_name);
                        snprintf(namebuff, _MAX_LFN * 2 + 1, "0:Audios\\%s", file_name);
                        startAnim(img_pre, "O-W", 1500, 100, 1, 7);
                        audio_play_state = AUDIO_PRE_PLAY;
                    }
                }else{
                    audio_play_state = AUDIO_RESUME;
                    startAnim(img_pre, "O-W", 1500, 100, 1, 7);
                }

            } else if (memcmp(lv_label_get_text(btn), LV_SYMBOL_PREV, 3) == 0) {
                DIR *dir = &SDDir;
                char *file_name;
                FRESULT state;
                if (file_index == 0) {
                    file_index = max_file_count;
                }
                file_index--;
                f_opendir(dir, (const TCHAR *) "Audios");
                state = exf_scan_files(dir, &file_name, NULL, file_index);
                f_closedir(dir);
                if (state == FR_OK) {
                    lv_label_set_text(title, file_name);
                    snprintf(namebuff, _MAX_LFN * 2 + 1, "0:Audios\\%s", file_name);
                    audio_play_state = AUDIO_CANCEL;
                    while (audio_play_current_state != AUDIO_NONE);
                    if (memcmp(img_pre->user_data, "W", 1) != 0) {
                        startAnim(img_pre, "O-W", 1500, 100, 1, 7);
                    } else {
                        startAnim(img_pre, "C", 500, 100, 1, 7);
                        startAnim(img_pre, "O-W", 1500, 400, 1, 7);
                    }
                    audio_play_state = AUDIO_PRE_PLAY;
                }
            } else if (memcmp(lv_label_get_text(btn), LV_SYMBOL_NEXT, 3) == 0) {
                DIR *dir = &SDDir;
                char *file_name;
                FRESULT state;
                file_index++;
                if (file_index >= max_file_count)file_index = 0;
                f_opendir(dir, (const TCHAR *) "Audios");
                state = exf_scan_files(dir, &file_name, NULL, file_index);
                f_closedir(dir);
                if (state == FR_OK) {
                    lv_label_set_text(title, file_name);
                    snprintf(namebuff, _MAX_LFN * 2 + 1, "0:Audios\\%s", file_name);
                    audio_play_state = AUDIO_CANCEL;
                    while (audio_play_current_state != AUDIO_NONE);
                    if (memcmp(img_pre->user_data, "W", 1) != 0) {
                        startAnim(img_pre, "O-W", 1500, 100, 1, 7);
                    } else {
                        startAnim(img_pre, "C", 500, 100, 1, 7);
                        startAnim(img_pre, "O-W", 1500, 400, 1, 7);
                    }
                    audio_play_state = AUDIO_PRE_PLAY;
                }
            } else if (memcmp(lv_label_get_text(btn), LV_SYMBOL_LIST, 3) == 0) {
                setup_ui(&ui_play, &ui_audio, "AUDIO");
            }
        } else {
            if (e->code == LV_EVENT_FOCUSED) {

            } else if (e->code == LV_EVENT_DEFOCUSED) {

            }
        }
    }
}

static void setup_screen(void *user_data) {
    audio_play_state = 0;
    ui_audio.screen = lv_obj_create(NULL);
    lv_obj_set_scrollbar_mode(ui_audio.screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_t *div = lv_obj_create(ui_audio.screen);
    lv_obj_set_flex_flow(div, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(div, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(div, LV_PCT(94), LV_PCT(94));
    lv_obj_set_pos(div, LV_PCT(3), LV_PCT(3));
    lv_obj_set_style_radius(div, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(div, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(div, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(div, 2, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(div, LV_SCROLLBAR_MODE_OFF);
    resume_screen(NULL);

    lv_obj_t *title = lv_label_create(div);
    lv_obj_set_size(title, LV_PCT(100), 22);
    lv_obj_set_pos(title, LV_PCT(0), LV_PCT(0));
    lv_label_set_text(title, "Hey!Music~");
    lv_label_set_long_mode(title, LV_LABEL_LONG_SCROLL);
    lv_obj_set_content_height(title, 25);
    lv_obj_set_style_border_side(title, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(title, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(title, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(title, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_text_color(title, lv_color_make(0xff, 0xff, 0xff), LV_PART_MAIN);


    lv_obj_t *img_div = lv_obj_create(div);
    lv_obj_set_size(img_div, LV_PCT(100), 100);
    lv_obj_set_style_pad_all(img_div, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(img_div, 2, LV_PART_MAIN);
    lv_obj_set_style_border_side(img_div, LV_BORDER_SIDE_TOP, LV_PART_MAIN);
    lv_obj_set_style_border_color(img_div, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(img_div, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *img_front = lv_img_create(img_div);
    lv_obj_set_size(img_front, 150, 100);
    lv_obj_center(img_front);
    lv_obj_t *img_pre = lv_img_create(img_div);
    lv_obj_set_size(img_pre, 150, 100);
    lv_obj_center(img_pre);

    lv_obj_t *progress_bar = lv_bar_create(div);
    lv_bar_set_range(progress_bar, 0, 100); // 设置进度条的范围
    lv_bar_set_value(progress_bar, 30, LV_ANIM_ON); // 设置进度条的当前值
    lv_obj_set_size(progress_bar, 150, 5);
    lv_obj_set_style_bg_color(progress_bar, lv_color_make(0, 0, 0), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(progress_bar, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_border_width(progress_bar, 1, LV_PART_MAIN);

    lv_obj_t *button_array = lv_obj_create(div);
    lv_obj_set_size(button_array, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(button_array, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(button_array, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_scrollbar_mode(button_array, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_all(button_array, 3, LV_PART_MAIN);
    lv_obj_t *btn;
    const char *mode_text[] = {LV_SYMBOL_PREV, LV_SYMBOL_PLAY, LV_SYMBOL_NEXT, "", LV_SYMBOL_LIST};
    for (int i = 0; i < 5; i++) {
        btn = lv_label_create(button_array);
        if (i == 3)
            lv_obj_set_size(btn, 10, 20);
        else
            lv_obj_set_size(btn, 20, 20);
        lv_obj_set_style_text_align(btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        lv_obj_set_style_pad_all(btn, 0, LV_PART_MAIN);
        lv_label_set_text(btn, mode_text[i]);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_style(btn, &style_screen_list_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_style(btn, &style_screen_list_extra_btns_main_pressed, LV_PART_MAIN | LV_STATE_PRESSED);
        lv_obj_add_style(btn, &style_screen_list_extra_btns_main_focused, LV_PART_MAIN | LV_STATE_FOCUSED);

        lv_obj_add_event_cb(btn, click_event, LV_EVENT_SHORT_CLICKED, NULL);
        lv_obj_add_event_cb(btn, click_event, LV_EVENT_LONG_PRESSED, NULL);
//        lv_obj_add_event_cb(btn, click_event, LV_EVENT_FOCUSED, NULL);
//        lv_obj_add_event_cb(btn, click_event, LV_EVENT_DEFOCUSED, NULL);
        if (i != 3)
            lv_group_add_obj(lv_group_get_default(), btn);
    }
    startAnim(img_front, "D", 1500, 500, 1, 9);

    lv_obj_set_style_img_opa(img_front, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_style_img_opa(img_pre, LV_OPA_0, LV_PART_MAIN);
    DIR *dir = &SDDir;
    FRESULT state;
    state = f_opendir(dir, (const TCHAR *) "Audios");
    uint16_t counts = 0;
    while (exf_scan_files(dir, NULL, NULL, 0) != FR_NO_FILE) {
        counts++;
    }
    f_closedir(dir);
    max_file_count = counts;
}
void my_refresh_cb(lv_disp_drv_t * disp_drv, const lv_area_t *ptr, lv_color_t *ptr1) {
    // 在这里执行屏幕刷新完成后的操作

}
static void resume_screen(void *user_data) {
    if (user_data) {
        uint16_t index = *(uint16_t *) user_data;
        lv_obj_t *title = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 0);
        if (title) {
            lv_obj_t *img_pre = NULL;
            lv_obj_t *img_div = lv_obj_get_child(lv_obj_get_child(ui_audio.screen, 0), 1);
            img_pre = lv_obj_get_child(img_div, 1);
            DIR *dir = &SDDir;
            char *file_name;
            FRESULT state;
            f_opendir(dir, (const TCHAR *) "Audios");
            state = exf_scan_files(dir, &file_name, NULL, index);
            f_closedir(dir);
            if (state == FR_OK || state == FR_NO_FILE) {
                file_index = index;
                lv_label_set_text(title, file_name);
                snprintf(namebuff, _MAX_LFN * 2 + 1, "0:Audios\\%s", file_name);
                audio_play_state = AUDIO_CANCEL;
                while (audio_play_current_state != AUDIO_NONE);
                if (memcmp(img_pre->user_data, "W", 1) != 0) {
                    startAnim(img_pre, "O-W", 1500, 100, 1, 7);
                } else {
                    startAnim(img_pre, "C", 500, 100, 1, 7);
                    startAnim(img_pre, "O-W", 1500, 400, 1, 7);
                }
                audio_play_state = AUDIO_PRE_PLAY;
            }
        }
        lv_mem_free(user_data);
        ui_audio.user_data = NULL;
    }
    else if(audio_play_state==AUDIO_PLAY){
        audio_play_state = AUDIO_HALT;
        while(audio_play_current_state!=AUDIO_HALT);
        audio_play_state = AUDIO_PLAY;
    }
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
}

static void delete_screen(void *user_data) {
    file_index = 0;
    max_file_count = 0;
    HeapManager.reset();
}