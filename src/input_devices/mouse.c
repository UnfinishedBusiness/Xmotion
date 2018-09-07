#include "mouse.h"
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

int mouse;

int current_x;
int current_y;
bool left_button_down;

void mouse_init_(void)
{
  current_x = 0;
  current_y = 0;
  left_button_down = false;
  if ((mouse = open ("/dev/input/mice", O_RDONLY|O_NONBLOCK)) == -1)
  {
      printf ("Could not open mouse device\n");
  }
}
void mouse_close(void)
{
  close(mouse);
}
void mouse_tick(void)
{
  int bytes;
  unsigned char data[3];
  int left, middle, right;
  signed char x, y;
  bytes = read(mouse, data, sizeof(data));
  if (bytes > 0)
  {
    left = data[0] & 0x1;
    right = data[0] & 0x2;
    middle = data[0] & 0x4;

    x = data[1];
    y = data[2];

    current_x += x;
    current_y -= y;

    left_button_down = left;
    //printf("x=%d, y=%d, left=%d, right=%d, middle=%d\n", x, y, left, right, middle);
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
