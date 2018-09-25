#include "mouse.h"
#include "main.h"
#include "gui/elements.h"

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

int mouse;

int current_x;
int current_y;
bool left_button_down;

void (*mouse_scroll_callback)(int) = NULL;

void mouse_init_(const char *mouse_device)
{
  current_x = 0;
  current_y = 0;
  left_button_down = false;
  printf("Opened mouse: %s\n", mouse_device);
  if ((mouse = open (mouse_device, O_RDONLY|O_NONBLOCK)) == -1)
  {
      printf ("Could not open mouse device\n");
  }
}
void mouse_disable_scroll_callback(void)
{
  mouse_scroll_callback = NULL;
}
void mouse_set_scroll_callback(void (*f)(int))
{
  mouse_scroll_callback = f;
}
void mouse_close(void)
{
  mouse_scroll_callback = NULL;
  close(mouse);
}
void mouse_tick(void)
{
  int nbytes;
  struct input_event event;
  nbytes = read(mouse, &event, sizeof(event));
  if (nbytes > 0)
  {
    //printf("event.type = %d, event.code = %d, event.value = %d\n", event.type, event.code, event.value);
    if (event.type == 2 && event.code == 8 && event.value == -1) //Scroll down
    {
      if (mouse_scroll_callback != NULL)
      {
        mouse_scroll_callback(-1);
      }
      else
      {
        gui_elements_viewer_zoom(-1 * gui_elements_viewer_get_zoom());
      }
    }
    else if (event.type == 2 && event.code == 8 && event.value == 1)
    {
      if (mouse_scroll_callback != NULL)
      {
        mouse_scroll_callback(1);
      }
      else
      {
        gui_elements_viewer_zoom(gui_elements_viewer_get_zoom());
      }
    }
    else if (event.type == 1 && event.code == 272) //left button up/down
    {
      left_button_down = event.value;
    }
    else if (event.type == 2 && event.code == 0) //X inc
    {
      if (left_button_down == 1 && current_x < 1262 && current_y > 87) //Click Dragging
      {
        gui_elements_viewer_pan_x(event.value);
      }
      current_x += event.value;
    }
    else if (event.type == 2 && event.code == 1) //Y inc
    {
      if (left_button_down == 1 && current_x < 1262 && current_y > 87)  //Click Dragging
      {
        gui_elements_viewer_pan_y(event.value);
      }
      current_y += event.value;
    }
    else
    {

    }
    //printf("Current_X = %d, Current_y = %d\n", current_x, current_y);
  }
}
void mouse_tick_(void)
{
  int bytes;
  unsigned char data[3];
  int left, middle, right, scroll_up, scroll_down;
  signed char x, y;
  bytes = read(mouse, data, sizeof(data));
  if (bytes > 0)
  {
    left = data[0] & 0x1;
    right = data[0] & 0x2;
    scroll_up = data[0] & 0x5;
    middle = data[0] & 0x4;

    x = data[1];
    y = data[2];

    current_x += x;
    current_y -= y;

    left_button_down = left;
    printf("x=%d, y=%d, left=%d, right=%d, middle=%d, scroll_up=%d\n", x, y, left, right, middle, scroll_up);
  }
}
int mouse_get_current_x(void)
{
  return current_x;
}
int mouse_get_current_y(void)
{
  return current_y;
}
bool mouse_read(lv_indev_data_t * data)
{
    /*Store the collected data*/
    data->point.x = current_x;
    data->point.y = current_y;

    data->state = left_button_down ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    return false;
}
