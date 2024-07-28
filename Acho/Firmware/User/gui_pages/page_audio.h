//
// Created by Mr.WU on 2023/10/2.
//

#ifndef LVGL_PAGE_AUDIO_H
#define LVGL_PAGE_AUDIO_H
#include "lvgl.h"
#include "gui_setup.h"

enum {
    AUDIO_NONE=0,
    AUDIO_PRE_PLAY,
    AUDIO_PLAY,
    AUDIO_HALT,
    AUDIO_RESUME,
    AUDIO_CANCEL
};

extern lv_ui ui_audio;
extern uint8_t audio_play_state;
#endif //LVGL_PAGE_AUDIO_H
