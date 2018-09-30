#include "plasma_control_ui.h"
#include "main.h"
#include "elements.h"

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

#define BACKGROUND_COLOR LV_COLOR_MAKE(10, 10, 10);
void set_background()
{
  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;                                 /*Set a great horizontal padding*/
  style.body.main_color = BACKGROUND_COLOR;
  style.body.grad_color = BACKGROUND_COLOR;

  /*Create an other container*/
  lv_obj_t * box1;
  box1 = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(box1, &style);     /*Set the new style*/
  lv_cont_set_fit(box1, true, false); /*Do not enable the vertical fit */
  //lv_obj_set_height(box2, 55);        /*Set a fix height*/
  lv_obj_set_size(box1, 1680, 1050);
}
void gui_plasma_control_ui_create()
{
  set_background();
  gui_elements_dro();
  gui_elements_controls();
  gui_elements_indicators();
  gui_elements_nav();
  gui_elements_viewer();
}
void gui_plasma_control_ui_close()
{
  gui_elements_dro_close();
  gui_elements_controls_close();
  gui_elements_indicators_close();
  gui_elements_nav_close();
  gui_elements_viewer_close();
}
