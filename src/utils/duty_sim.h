#ifndef DUTY_SIM_H
#define DUTY_SIM_H

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

/**
 * Create a keyboard application
 */
void duty_sim_init(void);
void duty_sim_tick(void);
void duty_sim_dro(bool);
void duty_sim_close(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DUTY_SIM_H*/
