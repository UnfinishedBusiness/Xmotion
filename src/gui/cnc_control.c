#include "cnc_control.h"
#include "elements.h"
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

lv_obj_t *dro;
float num = 1;
static lv_res_t btn_click_action(lv_obj_t * btn)
{
  uint8_t id = lv_obj_get_free_num(btn);

  //printf("Button %d is released\n", id);
  gui_elements_dro_close();
  //num = num * 1.3;
  //gui_elements_dro_update_wcs(num, num, num);
  //gui_elements_dro_update_abs(num, num, num);
  /* The button is released.
   * Make something here */

  return LV_RES_OK; /*Return OK if the button is not deleted*/
}

void gui_cnc_control_create()
{
  gui_elements_background();
  dro = gui_elements_dro();
  gui_elements_jog();

  /*Create a clear button*/
  //lv_obj_t *clr_btn = lv_btn_create(lv_scr_act(), NULL);
  //lv_cont_set_fit(clr_btn, true, true);
  //lv_btn_set_action(clr_btn, LV_BTN_ACTION_CLICK, btn_click_action);
  //lv_obj_t * btn_label = lv_label_create(clr_btn, NULL);
  //lv_label_set_text(btn_label, "Clear");
  //lv_obj_set_parent(dro, clr_btn);





}
