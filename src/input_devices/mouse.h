/**
 * @file keyboard.h
 *
 */

#ifndef MOUSE_H
#define MOUSE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lv_hal/lv_hal.h"
#include "../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a keyboard application
 */
bool mouse_read(lv_indev_data_t *);
int mouse_get_current_x(void);
int mouse_get_current_y(void);
void mouse_init_();
void mouse_tick(void);
void mouse_close(void);

void mouse_set_scroll_callback(void (*f)(int));
void mouse_disable_scroll_callback(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MOUSE_H*/
