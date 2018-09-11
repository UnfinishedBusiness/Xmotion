/**
 * @file keyboard.h
 *
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lv_ex_conf.h"

#include "../lvgl/lv_hal/lv_hal.h"
#include "../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
 struct keymap_t {
    char  letter;
    int  keycode;
    int  alt_mod;
    int  shift_mod;
    char special[20];
 };
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a keyboard application
 */
void keyboard_init(const char*);
void keyboard_tick(void);
char keyboard_keymap_lookup(int, int, int);
void keyboard_event(int, int);
bool keyboard_read(lv_indev_data_t * data);
void keyboard_close(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*KEYBOARD_H*/
