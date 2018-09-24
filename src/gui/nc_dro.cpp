#include "nc_dro.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
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

#define DRO_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define DRO_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *dro_container;
lv_obj_t *x_wcs_dro;
lv_obj_t *y_wcs_dro;
lv_obj_t *z_wcs_dro;
//lv_obj_t *x_abs_dro;
//lv_obj_t *y_abs_dro;
//lv_obj_t *z_abs_dro;

void gui_elements_dro_tick(void)
{
  if (dro_container != NULL)
  {
    gui_elements_dro_update_wcs(linuxcnc_position.dro.x, linuxcnc_position.dro.y, linuxcnc_position.dro.z);
  }
}
void gui_elements_dro_close()
{
  if (dro_container != NULL)
  {
    lv_obj_del(dro_container);
    dro_container = NULL;
  }
}
void gui_elements_dro_update_wcs(float x, float y, float z)
{
  if (dro_container != NULL)
  {
    char x_text[20];
    char y_text[20];
    char z_text[20];
    sprintf(x_text, "%0.4f", x);
    sprintf(y_text, "%0.4f", y);
    sprintf(z_text, "%0.4f", z);
    lv_label_set_text(x_wcs_dro, x_text);
    lv_label_set_text(y_wcs_dro, y_text);
    lv_label_set_text(z_wcs_dro, z_text);
  }
}
/*void gui_elements_dro_update_abs(float x, float y, float z)
{
  if (dro_container != NULL)
  {
    char x_text[20];
    char y_text[20];
    char z_text[20];
    sprintf(x_text, "%0.4f", x);
    sprintf(y_text, "%0.4f", y);
    sprintf(z_text, "%0.4f", z);
    lv_label_set_text(x_abs_dro, x_text);
    lv_label_set_text(y_abs_dro, y_text);
    lv_label_set_text(z_abs_dro, z_text);
  }
}*/
lv_obj_t *gui_elements_dro(void)
{
  static lv_style_t small_text_style;
  lv_style_copy(&small_text_style, &lv_style_plain);
  small_text_style.text.color = DRO_TEXT_COLOR;
  small_text_style.text.font = &lv_font_dejavu_10;

  static lv_style_t text_style;
  lv_style_copy(&text_style, &lv_style_plain);
  text_style.text.color = DRO_TEXT_COLOR;
  text_style.text.font = &lv_font_dejavu_40;

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = DRO_BACKGROUND_COLOR;
  style.body.grad_color = DRO_BACKGROUND_COLOR;

  /*Create an other container*/
  dro_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(dro_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(dro_container, 400, 300);
  lv_obj_align(dro_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);
  //Left Side
  lv_obj_t *x_static = lv_label_create(dro_container, NULL);
  lv_obj_set_style(x_static, &text_style);
  lv_label_set_text(x_static, "X");
  lv_obj_align(x_static, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

  lv_obj_t *y_static = lv_label_create(dro_container, NULL);
  lv_obj_set_style(y_static, &text_style);
  lv_label_set_text(y_static, "Y");
  lv_obj_align(y_static, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 120);

  lv_obj_t *z_static = lv_label_create(dro_container, NULL);
  lv_obj_set_style(z_static, &text_style);
  lv_label_set_text(z_static, "Z");
  lv_obj_align(z_static, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 230);
  //Right side, wcs
  x_wcs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(x_wcs_dro, &text_style);
  lv_label_set_text(x_wcs_dro, "0.0000");
  lv_obj_align(x_wcs_dro, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 10);

  y_wcs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(y_wcs_dro, &text_style);
  lv_label_set_text(y_wcs_dro, "0.0000");
  lv_obj_align(y_wcs_dro, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 120);

  z_wcs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(z_wcs_dro, &text_style);
  lv_label_set_text(z_wcs_dro, "0.0000");
  lv_obj_align(z_wcs_dro, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 230);

  //Left side, abs
  /*x_abs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(x_abs_dro, &small_text_style);
  lv_label_set_text(x_abs_dro, "0.0000");
  lv_obj_align(x_abs_dro, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 30);

  y_abs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(y_abs_dro, &small_text_style);
  lv_label_set_text(y_abs_dro, "0.0000");
  lv_obj_align(y_abs_dro, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 140);

  z_abs_dro = lv_label_create(dro_container, NULL);
  lv_obj_set_style(z_abs_dro, &small_text_style);
  lv_label_set_text(z_abs_dro, "0.0000");
  lv_obj_align(z_abs_dro, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 250);*/


  return dro_container;
}
