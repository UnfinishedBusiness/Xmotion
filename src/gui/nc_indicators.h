#ifndef NC_INDICATORS_H
#define NC_INDICATORS_H

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

/**********************
 * INDICATOR PROTOTYPES
 **********************/
lv_obj_t *gui_elements_indicators(void);
void gui_elements_indicators_close();
void gui_elements_indicators_set_led(int, bool);
void gui_elements_indicators_tick(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_INDICATORS_H*/
