// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.11
// Project name: BLE_test

#ifndef _BLE_TEST_UI_H
#define _BLE_TEST_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
void ui_event_Button2(lv_event_t * e);
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui____initial_actions0;
extern int LED_status;


LV_IMG_DECLARE(ui_img_394551424);    // assets/lamp-svg.png






void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
