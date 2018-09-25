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
 * DRO PROTOTYPES
 **********************/
lv_obj_t *gui_elements_dro(void);
void gui_elements_dro_update_wcs(float, float, float);
void gui_elements_dro_update_abs(float, float, float);
void gui_elements_dro_tick(void);
void gui_elements_dro_close();
void gui_elements_dro_homed(void);
void gui_elements_dro_unhomed(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_DRO_H*/
