#ifndef NC_NAV_H
#define NC_NAV_H

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
 * NAV PROTOTYPES
 **********************/
lv_obj_t *gui_elements_nav(void);
void gui_elements_nav_tick();
void gui_elements_nav_close();
void gui_elements_nav_register_item(const char*, const char*, const char*);
duk_ret_t javascript_gui_elements_nav_register_item(duk_context *);
duk_ret_t javascript_gui_elements_nav_first_close_item(duk_context *);
duk_ret_t javascript_gui_elements_nav(duk_context *);
duk_ret_t javascript_gui_elements_nav_close(duk_context *);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_NAV_H*/
