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
    void *user_data;
    void (*gui_setup_func)(void * user_data);
    void (*gui_resume_func)(void *user_data);
    void (*gui_delete_func)(void * user_data);
}lv_ui;

typedef struct Heap_Typedef{
    uint8_t is_init:1;
    uint8_t **data_heap;
    uint8_t *current_sec;
    uint32_t sec_total;
    uint32_t total;
    uint32_t index;
    struct Heap_Typedef * (*init)(uint32_t size);
    struct Heap_Typedef * (*reset)(void);
    struct Heap_Typedef * (*clear)(void);
    uint8_t * (*push)(void * data, uint32_t size);
}Heap_Typedef;
extern Heap_Typedef HeapManager;
extern lv_ui ui_ROOT;
void ui_init_style(lv_style_t * style);
void setup_ui(lv_ui *ui,lv_ui *last_ui,void *user_data);
void delete_ui(lv_ui *ui,void *user_data);
void GUIInit(void);

extern lv_style_t style_screen_list_extra_btns_main_default;
extern lv_style_t style_screen_list_extra_btns_main_pressed;
extern lv_style_t style_screen_list_extra_btns_main_focused;
#endif //INK_PAPER_GUI_SETUP_H
