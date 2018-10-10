#ifndef NC_DRO_H
#define NC_DRO_H

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
#define DRO_JOG_MODE 0
#define DRO_AUTO_MODE 1
#define DRO_MDI_MODE 2
#define DRO_ABORT_MODE 3
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 * DRO PROTOTYPES
 **********************/
lv_obj_t *gui_elements_dro(void);
void gui_elements_dro_update_wcs(float, float, float);
void gui_elements_dro_update_abs(float, float, float);
void gui_elements_dro_set_machine_state_indicator(int);
void gui_elements_dro_tick(void);
void gui_elements_dro_close();
void gui_elements_dro_homed(void);
void gui_elements_dro_unhomed(void);
duk_ret_t javascript_gui_elements_dro(duk_context *);
duk_ret_t javascript_gui_elements_dro_close(duk_context *);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_DRO_H*/
