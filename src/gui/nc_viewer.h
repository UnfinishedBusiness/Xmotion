#ifndef NC_VIEWER_H
#define NC_VIEWER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../lvgl/lv_hal/lv_hal.h"
#include "../lvgl/lvgl.h"
#include "javascript_vm/duktape.h" //For duk_ret_t and duk_context

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
typedef struct
{
    float x;
    float y;
}viewer_point_t;

typedef struct {
  lv_obj_t *obj; //address of lv_obj_t
  viewer_point_t mcs_points[1024];
  viewer_point_t matrix_points[1024]; //gets rounded down to lv_point_t in viewer_tick
  lv_point_t coord_points[1024]; //these points are int, non-floating point
  int number_of_points;

  lv_point_t clipped_coord_points[1024]; //these points are int, non-floating point
  int clipped_number_of_points;
} ViewerEntity;

lv_obj_t *gui_elements_viewer(void);
void gui_elements_viewer_tick(void);
void gui_elements_viewer_pan_x(int);
void gui_elements_viewer_pan_y(int);
void gui_elements_viewer_zoom(int);
float gui_elements_viewer_get_zoom(void);
size_t gui_elements_viewer_addEntitity(viewer_point_t[2048], int, char*);
void gui_elements_viewer_open_drawing(const char*);
void gui_elements_viewer_close_drawing();
void gui_elements_viewer_set_redraw_flag();
void gui_elements_viewer_close();
duk_ret_t javascript_gui_elements_viewer(duk_context *);
duk_ret_t javascript_gui_elements_viewer_close(duk_context *);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*NC_VIEWER_H*/
