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
#include "javascript_vm/duktape.h"
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
void gui_elements_indicators_set_floating_head_led(bool);
void gui_elements_indicators_set_torch_on_led(bool);
void gui_elements_indicators_set_arc_ok_led(bool);
void gui_elements_indicators_set_torch_up_led(bool);
void gui_elements_indicators_set_torch_down_led(bool);
void gui_elements_indicators_tick(void);
duk_ret_t javascript_gui_elements_indicators(duk_context *);
duk_ret_t javascript_gui_elements_indicators_close(duk_context *);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_INDICATORS_H*/
