#ifndef NC_ELEMENTS_H
#define NC_ELEMENTS_H

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

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void gui_elements_background(void);

/**********************
 * DRO PROTOTYPES
 **********************/
 //Digital Read Out
lv_obj_t *gui_elements_dro(void);
void gui_elements_dro_update_wcs(float, float, float);
void gui_elements_dro_update_abs(float, float, float);
void gui_elements_dro_close();

//Jog Speed Slider, Jog Cont., 0.010, 0.001, 0.0001 Steps...
lv_obj_t *gui_elements_jog(void);
void gui_elements_jog_close();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_ELEMENTS_H*/
