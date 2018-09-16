/**
 * @file hardware_utils.h
 *
 */

#ifndef HARDWARE_UTILS_H
#define HARDWARE_UTILS_H

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
void hardware_utils_set_graphics_mode(void);
void hardware_utils_set_text_mode(void);
void hardware_utils_print_screensize(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*HARDWARE_UTILS_H*/
