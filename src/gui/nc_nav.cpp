#include "nc_nav.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "utils/hardware_utils.h"
#include "javascript_vm/javascript.h"
#include "main.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
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

#define NAV_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define NAV_TEXT_COLOR LV_COLOR_MAKE(0, 255, 0);
#define NAV_WHITE_TEXT_COLOR LV_COLOR_MAKE(255, 255, 255);
lv_obj_t *nav_container;

lv_obj_t *nav_clock;
lv_obj_t *nav_clock_label;

lv_obj_t *nav_timer;
lv_obj_t *nav_timer_label;

lv_style_t nav_text_style;
size_t nav_timer_ms;
long long nav_timer_stamp;

typedef struct
{
    char name[100];
    char javascript_open_eval[1024];
    char javascript_close_eval[1024];
}elements_nav_t;

vector<elements_nav_t> nav_items;
string next_close_eval;

static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt)
{
  printf("Button: %s released\n", txt);
  for (size_t x = 0; x < nav_items.size(); x++)
  {
    if (strcmp(nav_items[x].name, txt) != 0)
    {
      printf("\tClosing %s\n", next_close_eval);
      javascript_modules_eval(next_close_eval.c_str());
      printf("\tEvaling: %s\n", nav_items[x].javascript_open_eval);
      javascript_modules_eval(nav_items[x].javascript_open_eval);
      next_close_eval = string(nav_items[x].javascript_close_eval);
    }
  }
  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}
void gui_elements_nav_register_item(const char* name, const char* javascript_open_eval, const char* javascript_close_eval)
{
  elements_nav_t n;
  sprintf(n.name, "\222%s", name);
  sprintf(n.javascript_open_eval, "%s", javascript_open_eval);
  sprintf(n.javascript_close_eval, "%s", javascript_close_eval);
  nav_items.push_back(n);
}
void gui_elements_nav_tick()
{
  if (nav_container != NULL)
  {
    nav_timer_ms += current_timestamp() - nav_timer_stamp;
    lv_label_set_text(nav_timer, millisToHuman(nav_timer_ms).c_str());
    nav_timer_stamp = current_timestamp();
    lv_label_set_text(nav_clock, currentDateTime().c_str());
  }
}
lv_obj_t *gui_elements_nav(void)
{
  nav_timer_ms = 0;
  nav_timer_stamp = current_timestamp();
  /*Create a button descriptor string array*/
  #define MAX_NAV_ITEMS 10
  static const char *btnm_map[MAX_NAV_ITEMS + 1]; //Allow up to 10 nav items + ""
  int number_of_items = (int)nav_items.size();
  if (number_of_items >= MAX_NAV_ITEMS)
  {
    number_of_items = MAX_NAV_ITEMS;
  }
  for (int x = 0; x < number_of_items; x++)
  {
    btnm_map[x] = nav_items[x].name;
    //printf("Constructing %s\n", btnm_map[x]);
  }
  btnm_map[number_of_items] = "";

  lv_style_copy(&nav_text_style, &lv_style_plain);
  nav_text_style.text.color = NAV_WHITE_TEXT_COLOR;
  nav_text_style.text.font = &lv_font_dejavu_20;

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

  nav_clock = lv_label_create(nav_container, NULL);
  lv_obj_set_style(nav_clock, &nav_text_style);
  lv_label_set_text(nav_clock, currentDateTime().c_str());
  lv_obj_align(nav_clock, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 30);

  nav_clock_label = lv_label_create(nav_container, NULL);
  lv_obj_set_style(nav_clock_label, &nav_text_style);
  lv_label_set_text(nav_clock_label, "Time");
  lv_obj_align(nav_clock_label, NULL, LV_ALIGN_IN_TOP_RIGHT, -40, 5);

  nav_timer = lv_label_create(nav_container, NULL);
  lv_obj_set_style(nav_timer, &nav_text_style);
  lv_label_set_text(nav_timer, millisToHuman(nav_timer_ms).c_str());
  lv_obj_align(nav_timer, NULL, LV_ALIGN_IN_TOP_RIGHT, -160, 30);

  nav_timer_label = lv_label_create(nav_container, NULL);
  lv_obj_set_style(nav_timer_label, &nav_text_style);
  lv_label_set_text(nav_timer_label, "Cycle");
  lv_obj_align(nav_timer_label, NULL, LV_ALIGN_IN_TOP_RIGHT, -150, 5);

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
