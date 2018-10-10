#ifndef NC_CONTROLS_H
#define NC_CONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lv_hal/lv_hal.h"
#include "../lvgl/lvgl.h"
#include "javascript_vm/duktape.h" //For duk_ret_t and duk_context

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
 * CONTROLS PROTOTYPES
 **********************/

//Jog Speed Slider and buttons
lv_obj_t *gui_elements_controls(void);
void gui_elements_controls_close();
duk_ret_t javascript_gui_elements_controls(duk_context *);
duk_ret_t javascript_gui_elements_controls_close(duk_context *);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_CONTROLS_H*/
