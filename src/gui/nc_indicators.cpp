#include "elements.h"
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

#define INDICATORS_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define INDICATORS_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *indicators_container;
#define INDICATOR_COUNT 5
lv_obj_t *indicator[INDICATOR_COUNT];
void gui_elements_indicators_tick(void)
{
  gui_elements_indicators_set_led(0, linuxcnc_get_status_bool("estop"));
  gui_elements_indicators_set_led(1, linuxcnc_get_status_bool("spindle_enabled"));
  gui_elements_indicators_set_led(2, linuxcnc_get_status_bool("probe_val"));
  gui_elements_indicators_set_led(3, false);
  gui_elements_indicators_set_led(4, false);
}
void gui_elements_indicators_set_led(int index, bool state)
{
  if (indicators_container == NULL) return;
  if (index > INDICATOR_COUNT -1) return;//0-4
  if (state)
  {
    lv_led_on(indicator[index]);
  }
  else
  {
    lv_led_off(indicator[index]);
  }
}
lv_obj_t *gui_elements_indicators(void)
{
  static lv_style_t small_text_style;
  lv_style_copy(&small_text_style, &lv_style_plain);
  small_text_style.text.color = INDICATORS_TEXT_COLOR;
  small_text_style.text.font = &lv_font_dejavu_10;

  static lv_style_t text_style;
  lv_style_copy(&text_style, &lv_style_plain);
  text_style.text.color = INDICATORS_TEXT_COLOR;
  text_style.text.font = &lv_font_dejavu_40;

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = INDICATORS_BACKGROUND_COLOR;
  style.body.grad_color = INDICATORS_BACKGROUND_COLOR;

  /*Create an other container*/
  indicators_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(indicators_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(indicators_container, 400, 80);
  lv_obj_align(indicators_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 320);

  /*Create a style for the LEDs*/
  static lv_style_t style_led;
  lv_style_copy(&style_led, &lv_style_pretty_color);
  style_led.body.radius = LV_RADIUS_CIRCLE;
  style_led.body.main_color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
  style_led.body.grad_color = LV_COLOR_MAKE(0x50, 0x07, 0x02);
  style_led.body.border.color = LV_COLOR_MAKE(0xfa, 0x0f, 0x00);
  style_led.body.border.width = 3;
  style_led.body.border.opa = LV_OPA_40;
  style_led.body.shadow.color = LV_COLOR_MAKE(0xb5, 0x0f, 0x04);
  style_led.body.shadow.width = 10;

  //Torch Led
  static lv_obj_t *ind1 = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(ind1, &small_text_style);
  lv_label_set_text(ind1, "ESTOP");
  lv_obj_align(ind1, NULL, LV_ALIGN_IN_TOP_LEFT, 17, 10);
  /*Create a LED and switch it ON*/
  indicator[0]  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(indicator[0], &style_led);
  lv_obj_align(indicator[0], NULL, LV_ALIGN_IN_BOTTOM_LEFT, 15, -15);
  lv_led_off(indicator[0]);

  //Floating Head Led
  static lv_obj_t *ind2 = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(ind2, &small_text_style);
  lv_label_set_text(ind2, "Spindle");
  lv_obj_align(ind2, NULL, LV_ALIGN_IN_TOP_LEFT, 87, 10);
  /*Create a LED and switch it ON*/
  indicator[1]  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(indicator[1], &style_led);
  lv_obj_align(indicator[1], NULL, LV_ALIGN_IN_BOTTOM_LEFT, 89, -15);
  lv_led_off(indicator[1]);

  //Arc Ok Led
  static lv_obj_t *ind3 = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(ind3, &small_text_style);
  lv_label_set_text(ind3, "Probe");
  lv_obj_align(ind3, NULL, LV_ALIGN_IN_TOP_LEFT, 161, 10);
  /*Create a LED and switch it ON*/
  indicator[2]  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(indicator[2], &style_led);
  lv_obj_align(indicator[2], NULL, LV_ALIGN_IN_BOTTOM_LEFT, 159, -15);
  lv_led_off(indicator[2]);

  //Torch Up Led
  static lv_obj_t *ind4 = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(ind4, &small_text_style);
  lv_label_set_text(ind4, "Aux 1");
  lv_obj_align(ind4, NULL, LV_ALIGN_IN_TOP_LEFT, 223, 10);
  /*Create a LED and switch it ON*/
  indicator[3]  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(indicator[3], &style_led);
  lv_obj_align(indicator[3], NULL, LV_ALIGN_IN_BOTTOM_LEFT, 229, -15);
  lv_led_off(indicator[3]);

  //Torch Down Led
  static lv_obj_t *ind5 = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(ind5, &small_text_style);
  lv_label_set_text(ind5, "Aux 2");
  lv_obj_align(ind5, NULL, LV_ALIGN_IN_TOP_LEFT, 288, 10);
  /*Create a LED and switch it ON*/
  indicator[4]  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(indicator[4], &style_led);
  lv_obj_align(indicator[4], NULL, LV_ALIGN_IN_BOTTOM_LEFT, 301, -15);
  lv_led_off(indicator[4]);


  return indicators_container;
}
void gui_elements_indicators_close()
{
  if (indicators_container != NULL)
  {
    lv_obj_del(indicators_container);
    indicators_container = NULL;
  }
}
