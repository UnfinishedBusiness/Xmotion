#ifndef NC_OPEN_DIALOG_H
#define NC_OPEN_DIALOG_H

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
 * VIEWER PROTOTYPES
 **********************/

lv_obj_t *gui_elements_open_dialog(void);
void gui_elements_read_directories(void);
void gui_elements_open_dialog_close();

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_OPEN_DIALOG_H*/
