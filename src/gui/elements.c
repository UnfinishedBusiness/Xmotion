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

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define BACKGROUND_COLOR LV_COLOR_MAKE(10, 10, 10);
void gui_elements_background(void)
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
    float x_rel = linuxcnc_get_x_rel_position();
    float y_rel = linuxcnc_get_y_rel_position();
    float z_rel = linuxcnc_get_z_rel_position();
    gui_elements_dro_update_wcs(x_rel, y_rel, z_rel);
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
#define JOG_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define JOG_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *controls_container;
static lv_res_t slider_action(lv_obj_t * slider)
{
    //printf("New slider value: %d\n", lv_slider_get_value(slider));
    float jog_speed = mapfloat((float)lv_slider_get_value(slider), 0, 100, 0, config.max_jog_speed);
    linuxcnc_set_jog_speed(jog_speed);
    return LV_RES_OK;
}
/*Called when a button is released or long pressed*/
//static const char * btnm_map[] = {"\224Continuous", "\2220.1", "\2220.01", "\2220.001", "\n",
                           //"\222X=0", "\222Y=0", "\222Z=0", "\n",
                           //"\224Probe Z", "\222Go Home", "\222Home", "\n",
                           //"\222Torch On", "\222Torch Off", "\n",
                           //"\224Cycle Start", "\222Pause", "\222Stop", "\222Open", ""};
static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt)
{
  //printf("Button: %s released\n", txt);
  if (!strcmp("Home", txt))
  {
    terminal_eval("home");
  }
  else if (!strcmp("Torch On", txt))
  {
    printf("Torch on!\n");
    linuxcnc_mdi("S100M3");
  }
  else if (!strcmp("Torch Off", txt))
  {
    printf("Torch off!\n");
    linuxcnc_mdi("M5");
  }
  else if (!strcmp("Go Home", txt))
  {
    printf("Return Home!\n");
    linuxcnc_mdi("G53 G0 X0.010 Y0.010 Z-0.010");
  }
  else if (!strcmp("X=0", txt))
  {
    printf("X=0\n");
    linuxcnc_mdi("G92 X0");
  }
  else if (!strcmp("Y=0", txt))
  {
    printf("Y=0\n");
    linuxcnc_mdi("G92 Y0");
  }
  else if (!strcmp("Z=0", txt))
  {
    printf("Z=0\n");
    linuxcnc_mdi("G92 Z0");
  }

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}
lv_obj_t *gui_elements_controls(void)
{
  static lv_style_t jog_slider_style;
  lv_style_copy(&jog_slider_style, &lv_style_plain);
  jog_slider_style.body.shadow.width = 6;
  jog_slider_style.body.padding.hor = 5;
  jog_slider_style.body.main_color = DRO_BACKGROUND_COLOR;
  jog_slider_style.body.grad_color = DRO_BACKGROUND_COLOR;
  jog_slider_style.body.radius = LV_RADIUS_CIRCLE;

  lv_obj_t *jog_slider_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(jog_slider_container, &jog_slider_style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(jog_slider_container, 400, 100);
  lv_obj_align(jog_slider_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 330);
  /*Create a bar, an indicator and a knob style*/
  static lv_style_t style_bg;
  static lv_style_t style_indic;
  static lv_style_t style_knob;

  lv_style_copy(&style_bg, &lv_style_pretty);
  style_bg.body.main_color =  LV_COLOR_BLACK;
  style_bg.body.grad_color =  LV_COLOR_GRAY;
  style_bg.body.radius = LV_RADIUS_CIRCLE;
  style_bg.body.border.color = LV_COLOR_WHITE;

  lv_style_copy(&style_indic, &lv_style_pretty);
  style_indic.body.grad_color =  LV_COLOR_GREEN;
  style_indic.body.main_color =  LV_COLOR_LIME;
  style_indic.body.radius = LV_RADIUS_CIRCLE;
  style_indic.body.shadow.width = 10;
  style_indic.body.shadow.color = LV_COLOR_LIME;
  style_indic.body.padding.hor = 3;
  style_indic.body.padding.ver = 3;

  lv_style_copy(&style_knob, &lv_style_pretty);
  style_knob.body.radius = LV_RADIUS_CIRCLE;
  style_knob.body.opa = LV_OPA_70;
  style_knob.body.padding.ver = 10 ;

  /*Create a slider*/
  lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), NULL);
  lv_slider_set_style(slider2, LV_SLIDER_STYLE_BG, &style_bg);
  lv_slider_set_style(slider2, LV_SLIDER_STYLE_INDIC,&style_indic);
  lv_slider_set_style(slider2, LV_SLIDER_STYLE_KNOB, &style_knob);
  lv_slider_set_action(slider2, slider_action);
  lv_obj_align(slider2, NULL, LV_ALIGN_IN_TOP_RIGHT, -100, 370);
  float jog_speed = mapfloat(config.default_jog_speed, 0, config.max_jog_speed, 0, 100);
  lv_slider_set_value (slider2, (int)jog_speed);

  /*Create a label*/
  lv_obj_t * slider2_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(slider2_label, "Jog Speed");
  lv_obj_align(slider2_label, slider2, LV_ALIGN_IN_TOP_MID, 0, -25);

  /*Create a button descriptor string array*/
  static const char * btnm_map[] = {"\224Continuous", "\2220.1", "\2220.01", "\2220.001", "\n",
                             "\222X=0", "\222Y=0", "\222Z=0", "\n",
                             "\224Probe Z", "\222Go Home", "\222Home", "\n",
                             "\222Torch On", "\222Torch Off", "\n",
                             "\224Cycle Start", "\222Pause", "\222Stop", "\222Open", ""};

  /*Create a new style for the button matrix back ground*/
  static lv_style_t matrix_style_bg;
  lv_style_copy(&matrix_style_bg, &lv_style_plain);
  matrix_style_bg.body.main_color = LV_COLOR_SILVER;
  matrix_style_bg.body.grad_color = LV_COLOR_SILVER;
  matrix_style_bg.body.padding.hor = 0;
  matrix_style_bg.body.padding.ver = 0;
  matrix_style_bg.body.padding.inner = 0;

  /*Create 2 button styles*/
  static lv_style_t style_btn_rel;
  static lv_style_t style_btn_pr;
  lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
  style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
  style_btn_rel.body.grad_color = LV_COLOR_BLACK;
  style_btn_rel.body.border.color = LV_COLOR_SILVER;
  style_btn_rel.body.border.width = 1;
  style_btn_rel.body.border.opa = LV_OPA_50;
  style_btn_rel.body.radius = 0;

  lv_style_copy(&style_btn_pr, &style_btn_rel);
  style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
  style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
  style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);

  /*Create a second button matrix with the new styles*/
  lv_obj_t * btnm2 = lv_btnm_create(lv_scr_act(), NULL);
  lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BG, &matrix_style_bg);
  lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
  lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
  lv_btnm_set_map(btnm2, btnm_map);
  lv_btnm_set_action(btnm2, btnm_action);
  lv_obj_set_size(btnm2, 400, 490);
  lv_obj_align(btnm2, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);

  return controls_container;
}
void gui_elements_controls_close()
{
  if (controls_container != NULL)
  {
    lv_obj_del(controls_container);
    controls_container = NULL;
  }
}

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
  gui_elements_indicators_set_torch_on_led(linuxcnc_get_pin_state("thcud.torch-on"));
  gui_elements_indicators_set_torch_up_led(linuxcnc_get_pin_state("thcud.torch-up"));
  gui_elements_indicators_set_torch_down_led(linuxcnc_get_pin_state("thcud.torch-down"));
  gui_elements_indicators_set_arc_ok_led(linuxcnc_get_pin_state("thcud.arc-ok"));
  gui_elements_indicators_set_floating_head_led(linuxcnc_get_pin_state("motion.probe-input"));
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
  style.body.main_color = INDICATORS_BACKGROUND_COLOR;
  style.body.grad_color = INDICATORS_BACKGROUND_COLOR;

  /*Create an other container*/
  indicators_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(indicators_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(indicators_container, 360, 80);
  lv_obj_align(indicators_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 450);

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

#define NAV_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define NAV_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *nav_container;
lv_obj_t *gui_elements_nav(void)
{
  /*Create a button descriptor string array*/
  static const char * btnm_map[] = {"CNC", "CAD", "CAM", "CONFIG", ""};

  /*Create a new style for the button matrix back ground*/
  static lv_style_t matrix_style_bg;
  lv_style_copy(&matrix_style_bg, &lv_style_plain);
  matrix_style_bg.body.main_color = LV_COLOR_SILVER;
  matrix_style_bg.body.grad_color = LV_COLOR_SILVER;
  matrix_style_bg.body.padding.hor = 0;
  matrix_style_bg.body.padding.ver = 0;
  matrix_style_bg.body.padding.inner = 0;

  /*Create 2 button styles*/
  static lv_style_t style_btn_rel;
  static lv_style_t style_btn_pr;
  lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
  style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
  style_btn_rel.body.grad_color = LV_COLOR_BLACK;
  style_btn_rel.body.border.color = LV_COLOR_SILVER;
  style_btn_rel.body.border.width = 1;
  style_btn_rel.body.border.opa = LV_OPA_50;
  style_btn_rel.body.radius = 0;

  lv_style_copy(&style_btn_pr, &style_btn_rel);
  style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
  style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
  style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = NAV_BACKGROUND_COLOR;
  style.body.grad_color = NAV_BACKGROUND_COLOR;

  nav_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(nav_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(nav_container, 1250, 60);
  lv_obj_align(nav_container, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

  /*Create a second button matrix with the new styles*/
  lv_obj_t *btn = lv_btnm_create(nav_container, NULL);
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BG, &matrix_style_bg);
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
  lv_btnm_set_style(btn, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
  lv_btnm_set_map(btn, btnm_map);
  lv_btnm_set_action(btn, btnm_action);
  lv_obj_set_size(btn, 300, 50);
  lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 5);

  return nav_container;
}
void gui_elements_nav_close()
{
  if (nav_container != NULL)
  {
    lv_obj_del(nav_container);
    nav_container = NULL;
  }
}


#define VIEWER_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define VIEWER_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *viewer_container;
int viewer_offset[] = {10, 10}; //Offset from Bottom Left of container
float viewer_zoom = 1;
lv_obj_t *boundry;
lv_point_t machine_boundry[] = {{0, 0}, {0, 45}, {45, 45}, {45, 0}, {0, 0}};
lv_point_t zoomed_machine_boundry[] = {{0, 0}, {0, }, {0, 0}, {0, 0}, {0, 0}};

static lv_point_t line_points[] = {{5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10}};
static lv_style_t style_line_feed;
static lv_style_t style_line_rapid;
static lv_style_t style_line_boundry;

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
  style_line_feed.line.width = 2;

  lv_style_copy(&style_line_rapid, &lv_style_plain);
  style_line_rapid.line.color = LV_COLOR_MAKE(0, 255, 0);
  style_line_rapid.line.width = 2;


  boundry = lv_line_create(viewer_container, NULL);
  lv_line_set_style(boundry, &style_line_boundry);
  gui_elements_viewer_zoom(0);
  //lv_line_set_points(boundry, machine_boundry, 5);     /*Set the points*/
  return viewer_container;
}
void gui_elements_viewer_tick(void)
{
  if (viewer_container != NULL)
  {
    lv_line_set_points(boundry, zoomed_machine_boundry, 5);     /*Set the points*/
    lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
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
    if (viewer_zoom < 1) viewer_zoom = 1;
  }
  else
  {
    viewer_zoom -= zoom_small;
    if (viewer_zoom > 10) viewer_zoom = 10;
  }

  printf("Zoom: %0.4f\n", viewer_zoom);

  for (int x = 0; x < 5; x++)
  {
    zoomed_machine_boundry[x].x = fround(machine_boundry[x].x * viewer_zoom);
    zoomed_machine_boundry[x].y = fround(machine_boundry[x].y * viewer_zoom);
  }
}
void gui_elements_viewer_pan_x(int pan)
{
  if (viewer_container != NULL)
  {
    viewer_offset[0] += pan;
    lv_obj_align(boundry, NULL, LV_ALIGN_IN_BOTTOM_LEFT, viewer_offset[0], viewer_offset[1] * -1);
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
