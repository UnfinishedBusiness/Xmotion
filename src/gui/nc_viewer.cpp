#include "nc_viewer.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "utils/gcode_parser.h"
#include "geometry/geometry.h"
#include "main.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <linux/input.h>
#include <linux/kd.h>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

#define VIEWER_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define VIEWER_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *viewer_container;
int viewer_offset[] = {300, -1000}; //Offset from top Left of container
float viewer_zoom = 12;
size_t y_hair_index, x_hair_index, machine_boundry;
static lv_style_t style_line_feed;
static lv_style_t style_line_rapid;
static lv_style_t style_line_boundry;
static lv_style_t style_cross_hair;
std::vector<ViewerEntity> Entities;

void gui_elements_viewer_open_drawing(char* drawing)
{
  gcode_parse("test/gcode_parser/1.ngc");
  gcode_parse_moves();

  gcode_move_t move;
  viewer_point_t current_position;
  current_position.x = 0; //Should set this to dro_rel
  current_position.y = 0;
  for (size_t x = 0; x < gcode_get_move_count(); x++)
  {
    move = gcode_get_move(x);
    //printf("Move.g = %d\n", move.g);
    /*if (move.g == 0)
    {
      //printf("Rapid move to X%0.4f Y%0.4f\n", move.x, move.y);
      viewer_point_t line[] = {{current_position.x, current_position.y}, {move.x, move.y}};
      gui_elements_viewer_addEntitity(line, 2, "rapid");
    }*/
    if (move.g == 1)
    {
      //printf("Line move to X%0.4f Y%0.4f\n", move.x, move.y);
      viewer_point_t line[] = {{current_position.x, current_position.y}, {move.x, move.y}};
      gui_elements_viewer_addEntitity(line, 2, "feed");
    }
    //if (move.g == 2 ) printf("Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
    //if (move.g == 3 ) printf("Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
    current_position.x = move.x;
    current_position.y = move.y;
  }
}
lv_obj_t *gui_elements_viewer(void)
{
  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = VIEWER_BACKGROUND_COLOR;
  style.body.grad_color = VIEWER_BACKGROUND_COLOR;

  viewer_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(viewer_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(viewer_container, 1250, 950);
  lv_obj_align(viewer_container, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);


  lv_style_copy(&style_line_boundry, &lv_style_plain);
  style_line_boundry.line.color = LV_COLOR_MAKE(255, 0, 0);
  style_line_boundry.line.width = 2;

  lv_style_copy(&style_line_feed, &lv_style_plain);
  style_line_feed.line.color = LV_COLOR_MAKE(255, 255, 255);
  style_line_feed.line.width = 1;

  lv_style_copy(&style_line_rapid, &lv_style_plain);
  style_line_rapid.line.color = LV_COLOR_MAKE(0, 255, 0);
  style_line_rapid.line.width = 1;

  lv_style_copy(&style_cross_hair, &lv_style_plain);
  style_cross_hair.line.color = LV_COLOR_MAKE(0, 0, 255);
  style_cross_hair.line.width = 2;

  viewer_point_t boundry[] = { {0,0}, {0,45}, {45,45}, {45, 0}, {0, 0} };
  machine_boundry = gui_elements_viewer_addEntitity(boundry, 5, "boundry");

  viewer_point_t x_hair[] = { {-15,0}, {15,0} };
  viewer_point_t y_hair[] = { {0,-15}, {0,15} };
  y_hair_index = gui_elements_viewer_addEntitity(y_hair, 2, "crosshair");
  x_hair_index = gui_elements_viewer_addEntitity(x_hair, 2, "crosshair");

  gui_elements_viewer_zoom(0);


  return viewer_container;
}
size_t gui_elements_viewer_addEntitity(viewer_point_t *points, int count, char *type)
{
  DEBUG_PRINT(("addEntitity!\n"));
  ViewerEntity e;
  for (int x = 0; x < count; x++)
  {
    e.mcs_points[x].x = points[x].x;
    e.mcs_points[x].y = points[x].y;
  }
  e.number_of_points = count;
  e.obj = lv_line_create(viewer_container, NULL);
  if (e.obj != NULL)
  {
    DEBUG_PRINT(("\tLine created ok!\n"));
    if (!strcmp(type, "boundry"))
    {
      lv_line_set_style(e.obj, &style_line_boundry);
    }
    else if (!strcmp(type, "rapid"))
    {
      lv_line_set_style(e.obj, &style_line_rapid);
    }
    else if (!strcmp(type, "feed"))
    {
      lv_line_set_style(e.obj, &style_line_feed);
    }
    else if (!strcmp(type, "crosshair"))
    {
      lv_line_set_style(e.obj, &style_cross_hair);
    }
    else
    {
      lv_line_set_style(e.obj, &style_line_boundry);
    }
    DEBUG_PRINT(("\tLine style set ok!\n"));
    //lv_line_set_points(e.obj, e.matrix_points, e.number_of_points); //viewer_tick rounds floats into int16_t aka lv_coord_t!
    DEBUG_PRINT(("\tLine points set ok!\n"));
    lv_obj_align(e.obj, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    DEBUG_PRINT(("\tLine obj aligned ok!\n"));
    Entities.push_back(e);
    gui_elements_viewer_tick(); //This is a must otherwise there is a lot of "lv_line_create" objects allocated but none with set points and causes segfaults
    DEBUG_PRINT(("\tLine pushed back ok!\n"));
    return Entities.size() -1;
  }
  else
  {
    DEBUG_PRINT(("\tLine not pushed back ok!\n"));
    return -1;
  }
}
void gui_elements_viewer_tick(void)
{
  //DEBUG_PRINT(("Viewer Tick!\n"));
  if (viewer_container != NULL)
  {
    for (size_t x = 0; x < Entities.size(); x++)
    {
      if (x == x_hair_index || x == y_hair_index)
      {
        //DEBUG_PRINT(("\tIndex Hair translation!"));
        for (int i = 0; i < Entities[x].number_of_points; i++)
        {
          Entities[x].matrix_points[i].x = (Entities[x].mcs_points[i].x) + viewer_offset[0] + (linuxcnc_x_dro * viewer_zoom);
          Entities[x].matrix_points[i].y = ((Entities[x].mcs_points[i].y * -1) + ((viewer_offset[1] + (linuxcnc_y_dro * viewer_zoom)) * -1));

          Entities[x].coord_points[i].x = (lv_coord_t)round(Entities[x].matrix_points[i].x);
          Entities[x].coord_points[i].y = (lv_coord_t)round(Entities[x].matrix_points[i].y);
        }
        //DEBUG_PRINT(("\t\tOK\n"));
        //DEBUG_PRINT(("\t\tSet Points!"));
        lv_line_set_points(Entities[x].obj, Entities[x].coord_points, Entities[x].number_of_points);
        //DEBUG_PRINT(("\t\tOK\n"));
      }
      else
      {
        //DEBUG_PRINT(("\tStatic Entity translation!"));
        for (int i = 0; i < Entities[x].number_of_points; i++)
        {
          Entities[x].matrix_points[i].x = (Entities[x].mcs_points[i].x * viewer_zoom) + viewer_offset[0];
          Entities[x].matrix_points[i].y = ((Entities[x].mcs_points[i].y * -1) * viewer_zoom + (viewer_offset[1] * -1));

          Entities[x].coord_points[i].x = (lv_coord_t)round(Entities[x].matrix_points[i].x);
          Entities[x].coord_points[i].y = (lv_coord_t)round(Entities[x].matrix_points[i].y);
        }
        //DEBUG_PRINT(("\t\tOK\n"));
        //DEBUG_PRINT(("\t\tSet Points!"));
        lv_line_set_points(Entities[x].obj, Entities[x].coord_points, Entities[x].number_of_points);
        //DEBUG_PRINT(("\t\tOK\n"));
      }
    }
  }
}
float fround(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =37.6716    for rounding off value
    // then type cast to int so value is 3766
    // then divided by 100 so the value converted into 37.66
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}
float gui_elements_viewer_get_zoom(void)
{
  return viewer_zoom;
}
void gui_elements_viewer_zoom(int zoom_inc)
{
  bool direction = true; //true = zoom in, false = zoom out
  if (zoom_inc < 0)
  {
    direction = false;
    zoom_inc = abs(zoom_inc);
  }
  float zoom_small = fround((float)zoom_inc / 10);
  if (direction)
  {
    viewer_zoom += zoom_small;
    if (viewer_zoom > 200) viewer_zoom = 200;
  }
  else
  {
    viewer_zoom -= zoom_small;
    if (viewer_zoom < 1) viewer_zoom = 1;

  }
}
void gui_elements_viewer_pan_x(int pan)
{
  if (viewer_container != NULL)
  {
    viewer_offset[0] += pan;
    //lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
  }
}
void gui_elements_viewer_pan_y(int pan)
{
  if (viewer_container != NULL)
  {
    viewer_offset[1] -= pan;
    //lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
  }
}
void gui_elements_viewer_close()
{
  if (viewer_container != NULL)
  {
    lv_obj_del(viewer_container);
    viewer_container = NULL;
  }
}
