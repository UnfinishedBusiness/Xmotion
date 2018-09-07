#include "keyboard.h"
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

int keyboard;

void keyboard_init(void)
{
  if ((keyboard = open ("/dev/input/event1", O_RDONLY|O_NONBLOCK)) == -1)
  {
      printf ("Could not open keyboard device\n");
  }
}
void keyboard_close(void)
{
  close(keyboard);
}
void keyboard_tick(void)
{
  int rd;
  int value;
  int size = sizeof (struct input_event);
  int key = 0;
  struct input_event ev[1];
  rd = read(keyboard, ev, size * 1);
  value = ev[0].value;
  key = ev[0].code;
  if (key != 0 && value < 100)
  {
    //printf ("Keyboard Code[%d]\n", (ev[0].code));
    //printf ("Keyboard Value[%d]\n", (ev[0].value));
    keyboard_event(ev[0].code, ev[0].value);
  }
}
void keyboard_event(int keycode, int state)
{
  int down = 1;
  int up = 0;
  int repeat = 2;
  if (keycode == 1 && state == down) //Escape
  {
    printf("Bye!\n");
    kill_main();
    return;
  }
  if (keycode == 106) //Right Arrow
  {
    if  (state == down)
    {
      printf("Right Arrow Down!\n");
    }
    if (state == up)
    {
      printf("Right Arrow Up!\n");
    }
  }
  if (keycode == 105) //Left Arrow
  {
    if  (state == down)
    {
      printf("Left Arrow Down!\n");
    }
    if (state == up)
    {
      printf("Left Arrow Up!\n");
    }

  }
}
