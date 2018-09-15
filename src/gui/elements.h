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
//void gui_elements_dro_update_abs(float, float, float);
void gui_elements_dro_tick(void);
void gui_elements_dro_close();

//Jog Speed Slider and buttons
lv_obj_t *gui_elements_controls(void);
void gui_elements_controls_close();

//Indicator Lights
lv_obj_t *gui_elements_indicators(void);
void gui_elements_indicators_close();
void gui_elements_indicators_set_floating_head_led(bool);
void gui_elements_indicators_set_torch_on_led(bool);
void gui_elements_indicators_set_arc_ok_led(bool);
void gui_elements_indicators_set_torch_up_led(bool);
void gui_elements_indicators_set_torch_down_led(bool);
void gui_elements_indicators_tick(void);

//Nav Menu
lv_obj_t *gui_elements_nav(void);
void gui_elements_nav_close();

//Gcode Viewer
typedef struct {
  lv_obj_t *obj; //address of lv_obj_t
  lv_point_t points[2048]; //array of points, make sure to not exceed 2,048 points!
  lv_point_t zoomed_points[2048];
  int number_of_points;
} ViewerEntity;

lv_obj_t *gui_elements_viewer(void);
void gui_elements_viewer_tick(void);
void gui_elements_viewer_pan_x(int);
void gui_elements_viewer_pan_y(int);
void gui_elements_viewer_zoom(int);
float gui_elements_viewer_get_zoom(void);
int gui_elements_viewer_addEntitity(lv_point_t[2048], int);
void gui_elements_viewer_close();

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_ELEMENTS_H*/
