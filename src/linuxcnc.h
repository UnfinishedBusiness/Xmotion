#ifndef LINUXCNC_H
#define LINUXCNC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
 #include "lv_ex_conf.h"

 #include "lvgl/lv_hal/lv_hal.h"
 #include "lvgl/lvgl.h"
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
void linuxcnc_init(void);
void linuxcnc_close(void);

void linuxcnc_jog_x_plus(bool);
void linuxcnc_jog_x_minus(bool);
void linuxcnc_jog_y_plus(bool);
void linuxcnc_jog_y_minus(bool);
void linuxcnc_jog_z_plus(bool);
void linuxcnc_jog_z_minus(bool);

void linuxcnc_set_jog_speed(float);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LINUXCNC_H*/
