/**
 * @file hardware_utils.h
 *
 */

#ifndef HARDWARE_UTILS_H
#define HARDWARE_UTILS_H

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lv_hal/lv_hal.h"
#include "../lvgl/lvgl.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
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
const std::string currentDateTime();
const std::string millisToHuman(size_t);
long long current_timestamp();
long long current_seconds_timestamp();
/**********************
 *      MACROS
 **********************/

#endif /*HARDWARE_UTILS_H*/
