#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

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
 typedef struct
 {
      //System
     const char* screen_size;
     const char* screen_color;
     const char* post_directory;

     //motion
     float max_jog_speed;
     float default_jog_speed;
 } configuration;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern configuration config;

/**
 * Create a keyboard application
 */
void config_handler_init(void);
void config_handler_close(void);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*CONFIG_HANDLER_H*/
