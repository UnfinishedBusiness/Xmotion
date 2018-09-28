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
lv_obj_t *torch_led;
lv_obj_t *floating_head_led;
lv_obj_t *arc_ok_led;
lv_obj_t *torch_up_led;
lv_obj_t *torch_down_led;
void gui_elements_indicators_tick(void)
{
  gui_elements_indicators_set_torch_on_led(linuxcnc_get_pin_state((char*)"thcud.torch-on"));
  gui_elements_indicators_set_torch_up_led(linuxcnc_get_pin_state((char*)"thcud.torch-up"));
  gui_elements_indicators_set_torch_down_led(linuxcnc_get_pin_state((char*)"thcud.torch-down"));
  gui_elements_indicators_set_arc_ok_led(linuxcnc_get_pin_state((char*)"thcud.arc-ok"));
  gui_elements_indicators_set_floating_head_led(linuxcnc_get_pin_state((char*)"motion.probe-input"));
}
void gui_elements_indicators_set_floating_head_led(bool state)
{
  if (indicators_container == NULL) return;
  if (state)
  {
    lv_led_on(floating_head_led);
  }
  else
  {
      lv_led_off(floating_head_led);
  }
}
void gui_elements_indicators_set_torch_on_led(bool state)
{
  if (indicators_container == NULL) return;
  if (state)
  {
    lv_led_on(torch_led);
  }
  else
  {
      lv_led_off(torch_led);
  }
}
void gui_elements_indicators_set_arc_ok_led(bool state)
{
  if (indicators_container == NULL) return;
  if (state)
  {
    lv_led_on(arc_ok_led);
  }
  else
  {
      lv_led_off(arc_ok_led);
  }
}
void gui_elements_indicators_set_torch_up_led(bool state)
{
  if (indicators_container == NULL) return;
  if (state)
  {
    lv_led_on(torch_up_led);
  }
  else
  {
      lv_led_off(torch_up_led);
  }
}
void gui_elements_indicators_set_torch_down_led(bool state)
{
  if (indicators_container == NULL) return;
  if (state)
  {
    lv_led_on(torch_down_led);
  }
  else
  {
      lv_led_off(torch_down_led);
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
  lv_obj_t *torch = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(torch, &small_text_style);
  lv_label_set_text(torch, "Torch");
  lv_obj_align(torch, NULL, LV_ALIGN_IN_TOP_LEFT, 17, 10);
  /*Create a LED and switch it ON*/
  torch_led  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(torch_led, &style_led);
  lv_obj_align(torch_led, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 15, -15);
  lv_led_off(torch_led);

  //Floating Head Led
  lv_obj_t *floating_head = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(floating_head, &small_text_style);
  lv_label_set_text(floating_head, "Z Probe");
  lv_obj_align(floating_head, NULL, LV_ALIGN_IN_TOP_LEFT, 87, 10);
  /*Create a LED and switch it ON*/
  floating_head_led  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(floating_head_led, &style_led);
  lv_obj_align(floating_head_led, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 89, -15);
  lv_led_off(floating_head_led);

  //Arc Ok Led
  lv_obj_t *arc_ok = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(arc_ok, &small_text_style);
  lv_label_set_text(arc_ok, "Arc Ok");
  lv_obj_align(arc_ok, NULL, LV_ALIGN_IN_TOP_LEFT, 161, 10);
  /*Create a LED and switch it ON*/
  arc_ok_led  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(arc_ok_led, &style_led);
  lv_obj_align(arc_ok_led, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 159, -15);
  lv_led_off(arc_ok_led);

  //Torch Up Led
  lv_obj_t *torch_up = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(torch_up, &small_text_style);
  lv_label_set_text(torch_up, "Torch Up");
  lv_obj_align(torch_up, NULL, LV_ALIGN_IN_TOP_LEFT, 223, 10);
  /*Create a LED and switch it ON*/
  torch_up_led  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(torch_up_led, &style_led);
  lv_obj_align(torch_up_led, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 229, -15);
  lv_led_off(torch_up_led);

  //Torch Down Led
  lv_obj_t *torch_down = lv_label_create(indicators_container, NULL);
  lv_obj_set_style(torch_down, &small_text_style);
  lv_label_set_text(torch_down, "Torch Down");
  lv_obj_align(torch_down, NULL, LV_ALIGN_IN_TOP_LEFT, 288, 10);
  /*Create a LED and switch it ON*/
  torch_down_led  = lv_led_create(indicators_container, NULL);
  lv_obj_set_style(torch_down_led, &style_led);
  lv_obj_align(torch_down_led, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 301, -15);
  lv_led_off(torch_down_led);


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
