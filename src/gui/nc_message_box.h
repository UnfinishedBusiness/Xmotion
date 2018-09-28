#ifndef NC_MESSAGE_BOX_H
#define NC_MESSAGE_BOX_H

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
     const char *msg;
     int timeout; //in seconds!
     int width;
     int height;
     int posx;
     int posy;
     bool active;
 }message_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 * INDICATOR PROTOTYPES
 **********************/
lv_obj_t *gui_elements_message_box(int, int, const char *, int, int);
void gui_elements_message_box_push(int width, int height, const char *msg, int posx, int posy, int timeout);
void gui_elements_message_box_close();
void gui_elements_message_box_timeout(int);
void gui_elements_message_box_tick(void);

/**********************
 *      MACROS
 **********************/

#endif /*NC_MESSAGE_BOX_H*/
