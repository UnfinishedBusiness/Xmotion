#ifndef LINUXCNC_H
#define LINUXCNC_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
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

extern float linuxcnc_x_dro;
extern float linuxcnc_y_dro;

void linuxcnc_init(void);
void linuxcnc_close(void);

void linuxcnc_jog_x_plus(bool);
void linuxcnc_jog_x_minus(bool);
void linuxcnc_jog_y_plus(bool);
void linuxcnc_jog_y_minus(bool);
void linuxcnc_jog_z_plus(bool);
void linuxcnc_jog_z_minus(bool);

void linuxcnc_set_jog_speed(float);

float linuxcnc_get_x_rel_position(void);
float linuxcnc_get_y_rel_position(void);
float linuxcnc_get_z_rel_position(void);

float linuxcnc_get_pin_state(char *);

void linuxcnc_mdi(char *);
void linuxcnc_jog_mode(void);

bool linuxcnc_is_axis_homed(int);
void linuxcnc_home_axis(int);
void linuxcnc_unhome_axis(int);

void linuxcnc_abort(void);

void linuxcnc_program_open(char *file);


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LINUXCNC_H*/
