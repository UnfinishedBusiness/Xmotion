#include "elements.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "javascript_vm/duktape.h"
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

using namespace std;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#define JOG_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define JOG_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
lv_obj_t *jog_slider_container;
lv_obj_t *feed_rate_container;

lv_obj_t *jog_speed_label;
char jog_speed_label_text[100];
lv_obj_t *feed_rate_label;
char feed_rate_label_text[100];

int matrix_count;
static lv_obj_t *matrix[3]; //Allocate button matrix slots on the stack


static lv_res_t jog_slider_action(lv_obj_t * slider)
{
    //printf("New slider value: %d\n", lv_slider_get_value(slider));
    float jog_speed = mapfloat((float)lv_slider_get_value(slider), 0, 100, 0, config.max_jog_speed);
    sprintf(jog_speed_label_text, "Jog Speed - %d IPM", (int)round(jog_speed));
    linuxcnc_set_jog_speed(jog_speed);
    lv_label_set_text(jog_speed_label, jog_speed_label_text);
    return LV_RES_OK;
}
static lv_res_t feed_slider_action(lv_obj_t * slider)
{
    //printf("New slider value: %d\n", lv_slider_get_value(slider));
    float feed_rate = (float)lv_slider_get_value(slider);
    sprintf(feed_rate_label_text, "Feed Rate - %d%%", (int)round(feed_rate));
    //linuxcnc_set_feed_rate_overide(feed_rate);
    lv_label_set_text(feed_rate_label, feed_rate_label_text);
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
    terminal_eval((char*)"home");
  }
  else if (!strcmp("Continuous", txt))
  {

  }
  else if (!strcmp("Torch On", txt))
  {
    printf("Torch on!\n");
    linuxcnc_mdi((char*)"S100M3");
  }
  else if (!strcmp("Torch Off", txt))
  {
    printf("Torch off!\n");
    linuxcnc_mdi((char*)"M5");
  }
  else if (!strcmp("Go Home", txt))
  {
    printf("Return Home!\n");
    linuxcnc_mdi((char*)"G53 G0 X0.010 Y0.010 Z-0.010");
  }
  else if (!strcmp("X=0", txt))
  {
    printf("X=0\n");
    linuxcnc_mdi((char*)"G92 X0");
  }
  else if (!strcmp("Y=0", txt))
  {
    printf("Y=0\n");
    linuxcnc_mdi((char*)"G92 Y0");
  }
  else if (!strcmp("Z=0", txt))
  {
    printf("Z=0\n");
    linuxcnc_mdi((char*)"G92 Z0");
  }
  else if (!strcmp("Open", txt))
  {
    gui_elements_open_dialog();
  }
  else if (!strcmp("Cycle Start", txt))
  {
    linuxcnc_cycle_start(0);
  }
  else if (!strcmp("Stop", txt))
  {
    linuxcnc_abort();
  }
  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}
lv_obj_t *nc_controls_button_group(const char **btnm_map, bool is_toggle, int default_id, int width, int height, int xpos, int ypos)
{
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

  matrix[matrix_count] = lv_btnm_create(lv_scr_act(), NULL);
  lv_btnm_set_style(matrix[matrix_count], LV_BTNM_STYLE_BG, &matrix_style_bg);
  lv_btnm_set_style(matrix[matrix_count], LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
  lv_btnm_set_style(matrix[matrix_count], LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
  lv_btnm_set_map(matrix[matrix_count], btnm_map);
  lv_btnm_set_action(matrix[matrix_count], btnm_action);
  lv_obj_set_size(matrix[matrix_count], width, height);
  lv_obj_align(matrix[matrix_count], NULL, LV_ALIGN_IN_BOTTOM_RIGHT, xpos, ypos);
  if (is_toggle) lv_btnm_set_toggle(matrix[matrix_count], true, default_id);
  matrix_count++;
  return matrix[matrix_count];
}
lv_obj_t *gui_elements_controls(void)
{
  matrix_count = 0;
  static lv_style_t jog_slider_style;
  lv_style_copy(&jog_slider_style, &lv_style_plain);
  jog_slider_style.body.shadow.width = 6;
  jog_slider_style.body.padding.hor = 5;
  jog_slider_style.body.main_color = JOG_BACKGROUND_COLOR;
  jog_slider_style.body.grad_color = JOG_BACKGROUND_COLOR;
  jog_slider_style.body.radius = LV_RADIUS_CIRCLE;

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

  jog_slider_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(jog_slider_container, &jog_slider_style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(jog_slider_container, 400, 75);
  lv_obj_align(jog_slider_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 425);
  /*Create a slider*/
  lv_obj_t *jog_speed_slider = lv_slider_create(jog_slider_container, NULL);
  lv_slider_set_style(jog_speed_slider, LV_SLIDER_STYLE_BG, &style_bg);
  lv_slider_set_style(jog_speed_slider, LV_SLIDER_STYLE_INDIC,&style_indic);
  lv_slider_set_style(jog_speed_slider, LV_SLIDER_STYLE_KNOB, &style_knob);
  lv_slider_set_action(jog_speed_slider, jog_slider_action);
  lv_obj_align(jog_speed_slider, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
  float jog_speed = config.default_jog_speed;
  lv_slider_set_value (jog_speed_slider, (int)mapfloat((float)jog_speed, 0, config.max_jog_speed, 0, 100));
  /*Create a label*/
  jog_speed_label = lv_label_create(jog_slider_container, NULL);
  sprintf(jog_speed_label_text, "Jog Speed - %d IPM", (int)round(jog_speed));
  lv_label_set_text(jog_speed_label, jog_speed_label_text);
  lv_obj_align(jog_speed_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);





  feed_rate_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(feed_rate_container, &jog_slider_style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(feed_rate_container, 400, 75);
  lv_obj_align(feed_rate_container, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 515);
  /*Create a slider*/
  lv_obj_t *feed_rate_slider = lv_slider_create(feed_rate_container, NULL);
  lv_slider_set_style(feed_rate_slider, LV_SLIDER_STYLE_BG, &style_bg);
  lv_slider_set_style(feed_rate_slider, LV_SLIDER_STYLE_INDIC,&style_indic);
  lv_slider_set_style(feed_rate_slider, LV_SLIDER_STYLE_KNOB, &style_knob);
  lv_slider_set_action(feed_rate_slider, feed_slider_action);
  lv_obj_align(feed_rate_slider, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -10);
  float feed_rate = 100; //100% Percent default!
  lv_slider_set_value (feed_rate_slider, (int)feed_rate);
  /*Create a label*/
  feed_rate_label = lv_label_create(feed_rate_container, NULL);
  sprintf(feed_rate_label_text, "Feed Rate - %d%%", (int)round(feed_rate));
  lv_label_set_text(feed_rate_label, feed_rate_label_text);
  lv_obj_align(feed_rate_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);



  /*Create a button descriptor string array*/
  static const char * btnm_map[] = {"\222X=0", "\222Y=0", "\222Z=0", "\n",
                             "\224Probe Z", "\222Go Home", "\222Home", "\n",
                             "\224Cycle Start", "\222Pause", "\222Stop", "\222Open", ""};

   static const char* jog_map[] = {"\224Continuous", "\2220.1", "\2220.01", "\2220.001", ""};

   static const char* misc_map[] = {"\222Torch On", "\222Torch Off", ""};

  nc_controls_button_group(jog_map, true, 0,400, 75, -10, -360);
  nc_controls_button_group(misc_map, true, 1, 400, 75, -10, -290);
  nc_controls_button_group(btnm_map, false, 0, 400, 280, -10, -10);
  return NULL;
}
void gui_elements_controls_close()
{
  for (int x = 0; x < matrix_count; x++)
  {
    if (matrix[x] != NULL)
    {
      lv_obj_del(matrix[x]);
      matrix[x] = NULL;
    }
  }
  if (jog_slider_container != NULL)
  {
    lv_obj_del(jog_slider_container);
    jog_slider_container = NULL;
  }
  if (feed_rate_container != NULL)
  {
    lv_obj_del(feed_rate_container);
    feed_rate_container = NULL;
  }
}
duk_ret_t javascript_gui_elements_controls(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  gui_elements_controls();
	duk_push_number(ctx, 0);
	return 0;  /* one return value */
}
duk_ret_t javascript_gui_elements_controls_close(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  gui_elements_controls_close();
	duk_push_number(ctx, 0);
	return 0;  /* one return value */
}
