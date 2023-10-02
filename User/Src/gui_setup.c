#include "gui_setup.h"

static Heap_Typedef *heap_init(uint32_t size);

static Heap_Typedef *heap_reset(void);

static Heap_Typedef *heap_clear(void);

static uint8_t *heap_push(void *data, uint32_t size);
lv_ui ui_ROOT = {
        .screen=NULL,
        .last_ui=NULL,
        .is_init = false,
        .gui_setup_func = NULL,
        .gui_resume_func = NULL,
        .gui_delete_func = NULL,
};
Heap_Typedef HeapManager = {
        .is_init = 0,
        .data_heap = NULL,
        .current_sec = NULL,
        .sec_total = 0,
        .total = 0,
        .index = 0,
        .init = heap_init,
        .reset = heap_reset,
        .clear = heap_clear,
        .push = heap_push,
};
extern lv_indev_t *indev_encoder;

void ui_init_style(lv_style_t *style) {
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void setup_ui(lv_ui *ui, lv_ui *last_ui, void *user_data) {
    if(last_ui==NULL)return;
    if(last_ui==&ui_ROOT){
        if(ui_ROOT.is_init==false)
            return;
    }
    ui->last_ui = last_ui;
    if (ui->group != NULL) {
        lv_group_del(ui->group);
        ui->group = NULL;
        indev_encoder->group = NULL;
    }
    ui->group = lv_group_create();
    lv_group_set_default(ui->group);
    if (ui->gui_setup_func)
        ui->gui_setup_func(user_data);
    if (ui->group)
        lv_indev_set_group(indev_encoder, ui->group);
    ui->is_init = true;
    lv_scr_load(ui->screen);
}

void delete_ui(lv_ui *ui, void *user_data) {
    if(ui==&ui_ROOT)return;
    if (!ui->is_init)return;
    if (ui->gui_delete_func)
        ui->gui_delete_func(user_data);
    if (ui->group) {
        lv_group_del(ui->group);
        ui->group = NULL;
        indev_encoder->group = NULL; // internal bugs should add or hard fault.
    }
    lv_obj_del_async(ui->screen);
    ui->screen = NULL;
    ui->is_init = false;
    if (ui->last_ui->screen) {
        if(ui->last_ui->gui_resume_func)
            ui->last_ui->gui_resume_func(ui->last_ui->user_data);
        lv_scr_load(ui->last_ui->screen);
        if (ui->last_ui->group)
            lv_indev_set_group(indev_encoder, ui->last_ui->group);
    }
}

static Heap_Typedef *heap_init(uint32_t size) {
    heap_reset();
    if (HeapManager.is_init == 0) {
        HeapManager.sec_total = 1;
        HeapManager.total = size;
        HeapManager.data_heap = lv_mem_alloc(sizeof(Heap_Typedef *));
        if(HeapManager.data_heap){
            HeapManager.data_heap[0] = lv_mem_alloc(HeapManager.total);
            if (HeapManager.data_heap[0]) {
                memset(HeapManager.data_heap[0], 0, HeapManager.total);
                HeapManager.current_sec = HeapManager.data_heap[0];
                HeapManager.is_init = 1;
            }
        }
    }
    return &HeapManager;
}

static Heap_Typedef *heap_reset(void) {
    if (HeapManager.data_heap && HeapManager.sec_total != 0) {
        for (uint32_t i = 0; i < HeapManager.sec_total; i++) {
            lv_mem_free(HeapManager.data_heap[i]);
            HeapManager.data_heap[i] = NULL;
        }
        lv_mem_free(HeapManager.data_heap);
    }
    HeapManager.sec_total = 0;
    HeapManager.data_heap = NULL;
    HeapManager.current_sec = NULL;
    HeapManager.total = 0;
    HeapManager.index = 0;
    HeapManager.is_init = 0;
    return &HeapManager;
}

static Heap_Typedef *heap_clear(void) {
    if (HeapManager.is_init && HeapManager.data_heap) {
        for (uint32_t i = 0; i < HeapManager.sec_total; i++) {
            if (HeapManager.data_heap[i]) {
                memset(HeapManager.data_heap[i], 0, HeapManager.total);
            }
        }
        HeapManager.index = 0;
        HeapManager.current_sec = HeapManager.data_heap[0];
    }
    return &HeapManager;
}

static uint8_t *heap_push(void *data, uint32_t size) {
    if (size == 0 || HeapManager.is_init==0 || HeapManager.current_sec==NULL) return NULL;
    if (HeapManager.total - HeapManager.index - 1 < size) {
        HeapManager.sec_total++;
        HeapManager.data_heap = lv_mem_realloc(HeapManager.data_heap,HeapManager.sec_total*sizeof(Heap_Typedef *));
        if(HeapManager.data_heap){
            HeapManager.current_sec = lv_mem_alloc(HeapManager.total); //total = pre_total
            if(HeapManager.current_sec){
                memset(HeapManager.current_sec, 0, HeapManager.total);
                HeapManager.data_heap[HeapManager.sec_total-1] = HeapManager.current_sec;
                HeapManager.index = 0;
            }
        }
    }
    if(HeapManager.current_sec){
        lv_memcpy_small(&HeapManager.current_sec[HeapManager.index],data,size);
        HeapManager.index +=size;
        return &HeapManager.current_sec[HeapManager.index-size];
    }
    return NULL;
}


lv_style_t style_screen_list_extra_btns_main_default;
lv_style_t style_screen_list_extra_btns_main_pressed;
lv_style_t style_screen_list_extra_btns_main_focused;
void GUIInit(void){

    ui_ROOT.screen = lv_scr_act();
    ui_ROOT.is_init = true;
    //Set style state: LV_STATE_DEFAULT for style_screen_list_extra_btns_main_default
    ui_init_style(&style_screen_list_extra_btns_main_default);
    lv_style_set_radius(&style_screen_list_extra_btns_main_default, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_default, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_default, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_default, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_text_font(&style_screen_list_extra_btns_main_default, &lv_font_montserrat_18);

    //Set style state: LV_STATE_PRESSED for style_screen_list_extra_btns_main_pressed

    ui_init_style(&style_screen_list_extra_btns_main_pressed);
    lv_style_set_radius(&style_screen_list_extra_btns_main_pressed, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_pressed, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_pressed, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_pressed, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_screen_list_extra_btns_main_pressed, &lv_font_montserrat_18);

    //Set style state: LV_STATE_FOCUSED for style_screen_list_extra_btns_main_focused

    ui_init_style(&style_screen_list_extra_btns_main_focused);
    lv_style_set_radius(&style_screen_list_extra_btns_main_focused, 3);
    lv_style_set_bg_color(&style_screen_list_extra_btns_main_focused, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_opa(&style_screen_list_extra_btns_main_focused, 255);
    lv_style_set_text_color(&style_screen_list_extra_btns_main_focused, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_outline_color(&style_screen_list_extra_btns_main_focused, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_outline_opa(&style_screen_list_extra_btns_main_focused, 255);
    lv_style_set_outline_width(&style_screen_list_extra_btns_main_focused, 1);
    lv_style_set_text_font(&style_screen_list_extra_btns_main_focused, &lv_font_montserrat_18);
    lv_style_set_outline_pad(&style_screen_list_extra_btns_main_focused, 2);
}