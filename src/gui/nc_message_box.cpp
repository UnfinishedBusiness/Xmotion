#include "elements.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "utils/hardware_utils.h"
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

#define MESSAGE_BOX_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define MESSAGE_BOX_TEXT_COLOR LV_COLOR_MAKE(255, 0, 0);

lv_style_t message_text_style;
lv_obj_t *message_box_container;
size_t message_box_timer_timeout;
long long message_box_timer_stamp;
bool message_box_timer_enabled;


using namespace std;

message_t message;

void gui_elements_message_box_push(int width, int height, const char *msg, int posx, int posy, int timeout)
{
  message.width = width;
  message.height = height;
  message.msg = msg;
  message.posx = posx;
  message.posy = posy;
  message.timeout = timeout;
  message.active = true;
}
lv_obj_t *gui_elements_message_box(int width, int height, const char *msg, int posx, int posy)
{
  message_box_timer_enabled = false;
  message_box_timer_timeout = 0;
  lv_style_copy(&message_text_style, &lv_style_plain);
  message_text_style.text.color = MESSAGE_BOX_TEXT_COLOR;
  message_text_style.text.font = &lv_font_dejavu_30;

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = MESSAGE_BOX_BACKGROUND_COLOR;
  style.body.grad_color = MESSAGE_BOX_BACKGROUND_COLOR;

  /*Create an other container*/
  message_box_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(message_box_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(dro_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(message_box_container, width, height);
  lv_obj_align(message_box_container, NULL, LV_ALIGN_IN_TOP_LEFT, posx, posy);
  //Left Side
  static lv_obj_t *msg_label = lv_label_create(message_box_container, NULL);
  lv_obj_set_style(msg_label, &message_text_style);
  lv_label_set_text(msg_label, message.msg);
  lv_obj_align(msg_label, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);
}
void gui_elements_message_box_close()
{
  if (message_box_container != NULL)
  {
    lv_obj_del(message_box_container);
    message_box_container = NULL;
  }
}
void gui_elements_message_box_timeout(int duration_seconds)
{
  if (message_box_container != NULL)
  {
    message_box_timer_enabled = true;
    message_box_timer_timeout = current_seconds_timestamp();
    //printf("Duration Seconds: %d\n", duration_seconds);
    message_box_timer_timeout += duration_seconds;
    //printf("Current Time: %llu\n",current_seconds_timestamp());
    //printf("Timer Timeout: %llu\n",message_box_timer_timeout);
  }
}
void gui_elements_message_box_tick(void)
{
  if (message_box_container != NULL)
  {
    if (message.active == true)
    {
      printf("Opening message box!\n");
      message.active = false;
      gui_elements_message_box(message.width, message.height, message.msg, message.posx, message.posy);
      gui_elements_message_box_timeout(message.timeout);
    }
    if (message_box_timer_enabled == true)
    {
      if (current_seconds_timestamp() > message_box_timer_timeout)
      {
        //printf("Message Box timeout! %llu\n", message_box_timer_timeout);
        gui_elements_message_box_close();
      }
      else
      {
        //printf("Message Box not yet timed out! %llu\n", message_box_timer_timeout);
      }
    }
  }
}
