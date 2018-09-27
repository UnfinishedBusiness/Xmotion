#include "nc_viewer.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "utils/gcode_parser.h"
#include "geometry/geometry.h"
#include "geometry/clipper.h"
#include "input_devices/mouse.h"
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

#define VIEWER_WIDTH  1250
#define VIEWER_HEIGHT 950
#define VIEWER_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define VIEWER_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *viewer_container;
float mouse_x_mcs, mouse_y_mcs;
int viewer_offset[] = {300, -800}; //Offset from top Left of container
float viewer_zoom = 12;
bool viewer_redraw = false;
size_t y_hair_index, x_hair_index, machine_boundry;
static lv_style_t style_line_feed;
static lv_style_t style_line_rapid;
static lv_style_t style_line_boundry;
static lv_style_t style_cross_hair;
std::vector<ViewerEntity> Entities;

void prime_viewer_stack()
{
  viewer_point_t boundry[] = { {0,0}, {0,45}, {45,45}, {45, 0}, {0, 0} };
  machine_boundry = gui_elements_viewer_addEntitity(boundry, 5, "boundry");

  viewer_point_t x_hair[] = { {-15,0}, {15,0} };
  viewer_point_t y_hair[] = { {0,-15}, {0,15} };
  y_hair_index = gui_elements_viewer_addEntitity(y_hair, 2, "crosshair");
  x_hair_index = gui_elements_viewer_addEntitity(x_hair, 2, "crosshair");
}
void gui_elements_viewer_close_drawing()
{
  gui_elements_viewer_close();
  gui_elements_viewer();
}
void gui_elements_viewer_open_drawing(const char* drawing)
{
  if (viewer_container == NULL) return; //Can't draw if viewer is closed!
  gcode_parse(drawing);
  gcode_parse_moves();

  gcode_move_t move;
  viewer_point_t current_position;
  current_position.x = 0; //Should set this to dro_rel
  current_position.y = 0;
  for (size_t x = 0; x < gcode_get_move_count(); x++)
  {
    move = gcode_get_move(x);
    bool good_arc = true;
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
    if (move.g == 2 )
    {
      //printf("Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
      geo_arc_t arc;
      arc.center.x = current_position.x + move.i;
      arc.center.y = current_position.y + move.j;

      arc.start.x = current_position.x;
      arc.start.y = current_position.y;

      arc.end.x = move.x;
      arc.end.y =  move.y;

      geo_line_t line;
      line.start.x = move.x;
      line.start.y = move.y;

      line.end.x = arc.center.x;
      line.end.y = arc.center.y;

      arc.radius = geoGetLineLength(line);
      if (move.i == 0 && move.j == 0) good_arc = false;
      if (arc.radius > 0 && good_arc == true)
      {
        arc.direction = ARC_CW;
        //printf("\tarc.radius=%0.4f arc.start=(%0.4f, %0.4f) arc.end=(%0.4f, %0.4f) arc.center=(%0.4f, %0.4f)\n", arc.radius, arc.start.x, arc.start.y, arc.end.x, arc.end.y, arc.center.x, arc.center.y);
        std::vector<geo_point_t> vector_points = geoGetPointsOfArc(arc);
        viewer_point_t arc_points[2048];
        int num_p = 0;
        int end_p = (int)vector_points.size();
        if (vector_points.size() > 2048) end_p = 2048;
        for (size_t x = 0; x < end_p; x++)
        {
          arc_points[x].x = vector_points[x].x;
          arc_points[x].y = vector_points[x].y;
          num_p++;
        }
        gui_elements_viewer_addEntitity(arc_points, num_p, "feed");
      }
    }
    if (move.g == 3 )
    {
      //printf("Counter-Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
      geo_arc_t arc;
      arc.center.x = current_position.x + move.i;
      arc.center.y = current_position.y + move.j;

      arc.start.x = current_position.x;
      arc.start.y = current_position.y;

      arc.end.x = move.x;
      arc.end.y =  move.y;

      geo_line_t line;
      line.start.x = move.x;
      line.start.y = move.y;

      line.end.x = arc.center.x;
      line.end.y = arc.center.y;

      arc.radius = geoGetLineLength(line);
      if (move.i == 0 && move.j == 0) good_arc = false;
      if (arc.radius > 0 && good_arc == true)
      {
        arc.direction = ARC_CCW;
        //printf("\tarc.radius=%0.4f arc.start=(%0.4f, %0.4f) arc.end=(%0.4f, %0.4f) arc.center=(%0.4f, %0.4f)\n", arc.radius, arc.start.x, arc.start.y, arc.end.x, arc.end.y, arc.center.x, arc.center.y);
        std::vector<geo_point_t> vector_points = geoGetPointsOfArc(arc);
        viewer_point_t arc_points[2048];
        int num_p = 0;
        int end_p = (int)vector_points.size();
        if (vector_points.size() > 2048) end_p = 2048;
        for (size_t x = 0; x < end_p; x++)
        {
          arc_points[x].x = vector_points[x].x;
          arc_points[x].y = vector_points[x].y;
          num_p++;
        }
        gui_elements_viewer_addEntitity(arc_points, num_p, "feed");
      }
    }
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
  lv_obj_set_size(viewer_container, VIEWER_WIDTH, VIEWER_HEIGHT);
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

  prime_viewer_stack(); //Draw boundry and crosshair!

  gui_elements_viewer_zoom(0);


  return viewer_container;
}
size_t gui_elements_viewer_addEntitity(viewer_point_t *points, int count, char *type)
{
  viewer_redraw = true;
  DEBUG_PRINT(("addEntitity!\n"));
  ViewerEntity e;
  for (int x = 0; x < count; x++)
  {
    e.mcs_points[x].x = points[x].x;
    e.mcs_points[x].y = points[x].y;

    e.matrix_points[x].x = ((e.mcs_points[x].x + linuxcnc_position.work_offset.x + linuxcnc_position.g92_offset.x + linuxcnc_position.tool_offset.x) * viewer_zoom) + viewer_offset[0];
    e.matrix_points[x].y = (((e.mcs_points[x].y + linuxcnc_position.work_offset.y + linuxcnc_position.g92_offset.y + linuxcnc_position.tool_offset.y) * -1) * viewer_zoom + (viewer_offset[1] * -1));

    e.coord_points[x].x = (lv_coord_t)round(e.matrix_points[x].x);
    e.coord_points[x].y = (lv_coord_t)round(e.matrix_points[x].y);
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
    //gui_elements_viewer_tick(); //This is a must otherwise there is a lot of "lv_line_create" objects allocated but none with set points and causes segfaults. Don't call tick, big time waster!
    int a = Entities.size()-1;

    lv_line_set_points(e.obj, e.coord_points, e.number_of_points);
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
  //printf("x_hair_index: %d, y_hair_index: %d\n", x_hair_index, y_hair_index);

  if (viewer_container != NULL && viewer_redraw == true)
  {
    viewer_redraw = false;
    for (size_t x = 0; x < Entities.size(); x++)
    {
      if (x == x_hair_index || x == y_hair_index)
      {
        //printf("linuxcnc_x_dro = %0.4f, linuxcnc_y_dro = %0.4f\n", linuxcnc_x_dro, linuxcnc_y_dro);
        for (int i = 0; i < Entities[x].number_of_points; i++)
        {
          Entities[x].matrix_points[i].x = (Entities[x].mcs_points[i].x) + viewer_offset[0] + (linuxcnc_position.mcs.x * viewer_zoom);
          Entities[x].matrix_points[i].y = ((Entities[x].mcs_points[i].y * -1) + ((viewer_offset[1] + (linuxcnc_position.mcs.y * viewer_zoom)) * -1));

          Entities[x].coord_points[i].x = (lv_coord_t)round(Entities[x].matrix_points[i].x);
          Entities[x].coord_points[i].y = (lv_coord_t)round(Entities[x].matrix_points[i].y);
        }
        //DEBUG_PRINT(("\t\tOK\n"));
        //DEBUG_PRINT(("\t\tSet Points!"));
        lv_line_set_points(Entities[x].obj, Entities[x].coord_points, Entities[x].number_of_points);
        //DEBUG_PRINT(("\t\tOK\n"));
      }
      else if (x == machine_boundry)
      {
          //DEBUG_PRINT(("\tStatic Entity translation!"));
          //int points_out_of_bounds = 0;
          for (int i = 0; i < Entities[x].number_of_points; i++)
          {
            //Offset matrix_point for scaling and pan
            Entities[x].matrix_points[i].x = (Entities[x].mcs_points[i].x * viewer_zoom) + viewer_offset[0];
            Entities[x].matrix_points[i].y = ((Entities[x].mcs_points[i].y * -1) * viewer_zoom + (viewer_offset[1] * -1));

            Entities[x].coord_points[i].x = (lv_coord_t)round(Entities[x].matrix_points[i].x);
            Entities[x].coord_points[i].y = (lv_coord_t)round(Entities[x].matrix_points[i].y);
            /*if ((Entities[x].coord_points[i].x > VIEWER_WIDTH || Entities[x].coord_points[i].x < 0) && (Entities[x].coord_points[i].y < VIEWER_HEIGHT || Entities[x].coord_points[i].y > 0))
            {
              points_out_of_bounds++;
            }*/
          }
          /*if (points_out_of_bounds == Entities[x].number_of_points)
          {
            //printf("Entity %d is out of sight!\n");
            for (int i = 0; i < Entities[x].number_of_points; i++)
            {
              Entities[x].coord_points[i].x = 0;
              Entities[x].coord_points[i].y = 0;
            }
          }*/
          //DEBUG_PRINT(("\t\tOK\n"));
          //DEBUG_PRINT(("\t\tSet Points!"));
          lv_line_set_points(Entities[x].obj, Entities[x].coord_points, Entities[x].number_of_points);
          //DEBUG_PRINT(("\t\tOK\n"));
      }
      else
      {
        //DEBUG_PRINT(("\tStatic Entity translation!"));
        //Entities[x].clipped_number_of_points = 0;
        for (int i = 0; i < Entities[x].number_of_points; i++)
        {
          //Offset matrix_point for scaling and pan
          Entities[x].matrix_points[i].x = ((Entities[x].mcs_points[i].x + linuxcnc_position.work_offset.x + linuxcnc_position.g92_offset.x + linuxcnc_position.tool_offset.x) * viewer_zoom) + viewer_offset[0];
          Entities[x].matrix_points[i].y = (((Entities[x].mcs_points[i].y + linuxcnc_position.work_offset.y + linuxcnc_position.g92_offset.y + linuxcnc_position.tool_offset.y) * -1) * viewer_zoom + (viewer_offset[1] * -1));

          Entities[x].coord_points[i].x = (lv_coord_t)round(Entities[x].matrix_points[i].x);
          Entities[x].coord_points[i].y = (lv_coord_t)round(Entities[x].matrix_points[i].y);

          /*if ((Entities[x].coord_points[i].x > VIEWER_WIDTH || Entities[x].coord_points[i].x < 0) && (Entities[x].coord_points[i].y < VIEWER_HEIGHT || Entities[x].coord_points[i].y > 0))
          {
            //Out of bounds!
          }
          else
          {
            Entities[x].clipped_coord_points[i].x = Entities[x].coord_points[i].x;
            Entities[x].clipped_coord_points[i].y = Entities[x].coord_points[i].y;
            Entities[x].clipped_number_of_points++;
          }*/
        }
        //DEBUG_PRINT(("\t\tOK\n"));
        //DEBUG_PRINT(("\t\tSet Points!"));
        lv_line_set_points(Entities[x].obj, Entities[x].coord_points, Entities[x].number_of_points); //We do this after we use clipper to clip the paths around the view box
        //DEBUG_PRINT(("\t\tOK\n"));
      }
    }

    //printf("MouseX: %d, MouseY in MCS: %d\n", mouse_get_current_x(), (mouse_get_current_y() - LV_VER_RES) * -1);
    mouse_x_mcs = (((float)mouse_get_current_x() - viewer_offset[0]) / viewer_zoom);
    mouse_y_mcs = (((float)mouse_get_current_y() + viewer_offset[1] - (LV_VER_RES - VIEWER_HEIGHT)) / viewer_zoom) * -1;
    //printf("MouseX in MCS: %0.4f, MouseY in MCS: %0.4f\n", mouse_x_mcs, mouse_y_mcs);
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
  viewer_redraw = true;

  float old_zoom = viewer_zoom;

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
    if (viewer_zoom > 600) viewer_zoom = 600;
  }
  else
  {
    viewer_zoom -= zoom_small;
    if (viewer_zoom < 1) viewer_zoom = 1;
  }
  float scalechange = viewer_zoom - old_zoom;
  //printf("Scale change: %0.4f\n", scalechange);
  float pan_x = (mouse_x_mcs * scalechange) * -1;
  float pan_y = (mouse_y_mcs * scalechange);
  //printf("PanX: %0.4f, PanY: %0.4f\n", pan_x, pan_y);
  gui_elements_viewer_pan_x(pan_x);
  gui_elements_viewer_pan_y(pan_y);
}
void gui_elements_viewer_pan_x(int pan)
{
  viewer_redraw = true;
  if (viewer_container != NULL)
  {
    viewer_offset[0] += pan;
    //lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
  }
}
void gui_elements_viewer_pan_y(int pan)
{
  viewer_redraw = true;
  if (viewer_container != NULL)
  {
    viewer_offset[1] -= pan;
    //lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
  }
}
void gui_elements_viewer_set_redraw_flag()
{
  viewer_redraw = true;
}
void gui_elements_viewer_close()
{
  if (viewer_container != NULL)
  {
    lv_obj_del(viewer_container);
    viewer_container = NULL;
    Entities.clear();
  }
}
