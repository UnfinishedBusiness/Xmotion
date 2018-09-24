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
 typedef struct
 {
     float x;
     float y;
     float z;
 }linuxcnc_coordinate_t;
 typedef struct
 {
     linuxcnc_coordinate_t dro;
     linuxcnc_coordinate_t mcs;
     linuxcnc_coordinate_t work_offset;
     linuxcnc_coordinate_t g92_offset;
     linuxcnc_coordinate_t tool_offset;
 }linuxcnc_position_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/

extern linuxcnc_position_t linuxcnc_position;

void linuxcnc_init(void);
void linuxcnc_close(void);

void linuxcnc_jog_x_plus(bool);
void linuxcnc_jog_x_minus(bool);
void linuxcnc_jog_y_plus(bool);
void linuxcnc_jog_y_minus(bool);
void linuxcnc_jog_z_plus(bool);
void linuxcnc_jog_z_minus(bool);

void linuxcnc_set_jog_speed(float);

float linuxcnc_get_pin_state(char *);

void linuxcnc_mdi(char *);
void linuxcnc_jog_mode(void);

bool linuxcnc_is_axis_homed(int);
void linuxcnc_home_axis(int);
void linuxcnc_unhome_axis(int);

void linuxcnc_abort(void);

void linuxcnc_program_open(char *file);

void linuxcnc_tick();



/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LINUXCNC_H*/
