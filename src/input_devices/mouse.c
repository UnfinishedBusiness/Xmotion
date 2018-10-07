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
#include <dirent.h>

#define MAX_MOUSE_DEVICES 5
int mouse[MAX_MOUSE_DEVICES];
int mouse_count;

int current_x;
int current_y;
bool left_button_down;

void (*mouse_scroll_callback)(int) = NULL;
void (*mouse_right_click_callback)(int) = NULL;
void (*mouse_left_click_callback)(int) = NULL;

void mouse_init_()
{
  mouse_count = 0;
  char device[1024];

  current_x = 0;
  current_y = 0;
  left_button_down = false;

  DIR *dpdf;
  struct dirent *epdf;
  dpdf = opendir("/dev/input/by-id/");
  if (dpdf != NULL)
  {
    printf("Finding Mouse devices!\n");
     while (epdf = readdir(dpdf))
     {
        //printf("Scanning keyboard inputs! %s\n", epdf->d_name);
        if (strstr(epdf->d_name, "event-mouse") != NULL)
        {
          sprintf(device, "/dev/input/by-id/%s", epdf->d_name);
          printf("\tFound Device: %s\n", device);
          if ((mouse[mouse_count] = open (device, O_RDONLY|O_NONBLOCK)) == -1)
          {
              printf ("\t\tCould not open device!\n");
          }
          printf("\t\tOpened device!\n");
          mouse_count++;
          if (mouse_count == MAX_MOUSE_DEVICES) break;
        }
     }
  }
  closedir(dpdf);
}
void mouse_disable_scroll_callback(void)
{
  mouse_scroll_callback = NULL;
}
void mouse_set_scroll_callback(void (*f)(int))
{
  mouse_scroll_callback = f;
}
void mouse_disable_right_click_callback(void)
{
  mouse_right_click_callback = NULL;
}
void mouse_set_right_click_callback(void (*f)(int))
{
  mouse_right_click_callback = f;
}
void mouse_disable_left_click_callback(void)
{
  mouse_left_click_callback = NULL;
}
void mouse_set_left_click_callback(void (*f)(int))
{
  mouse_left_click_callback = f;
}
void mouse_close(void)
{
  mouse_scroll_callback = NULL;
  for(int x = 0; x < mouse_count; x++)
  {
      close(mouse[x]);
  }
}
void mouse_tick(void)
{
  for(int x = 0; x < mouse_count; x++)
  {
    int nbytes;
    struct input_event event;
    nbytes = read(mouse[x], &event, sizeof(event));
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
        if (mouse_left_click_callback != NULL)
        {
          mouse_left_click_callback(event.value);
        }
        else
        {
          left_button_down = event.value;
        }
      }
      else if (event.type == 1 && event.code == 273) //right button up/down
      {
        if (mouse_right_click_callback != NULL)
        {
          mouse_right_click_callback(event.value);
        }
      }
      else if (event.type == 2 && event.code == 0) //X inc
      {
        if (left_button_down == 1 && current_x < 1262 && current_y > 87) //Click Dragging
        {
          //gui_elements_viewer_pan_x(event.value);
        }
        current_x += event.value;
      }
      else if (event.type == 2 && event.code == 1) //Y inc
      {
        if (left_button_down == 1 && current_x < 1262 && current_y > 87)  //Click Dragging
        {
          //gui_elements_viewer_pan_y(event.value);
        }
        current_y += event.value;
      }
      else
      {

      }
      //printf("Current_X = %d, Current_y = %d\n", current_x, current_y);
    }
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
