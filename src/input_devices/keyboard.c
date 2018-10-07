#include "keyboard.h"
#include "linuxcnc.h"
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
#include <dirent.h>

#include "utils/terminal.h"


struct keymap_t keymap[] = {
    {'a', 30, 0, 0, ""},
    {'b', 48, 0, 0, ""},
    {'c', 46, 0, 0, ""},
    {'d', 32, 0, 0, ""},
    {'e', 18, 0, 0, ""},
    {'f', 33, 0, 0, ""},
    {'g', 34, 0, 0, ""},
    {'h', 35, 0, 0, ""},
    {'i', 23, 0, 0, ""},
    {'j', 36, 0, 0, ""},
    {'k', 37, 0, 0, ""},
    {'l', 38, 0, 0, ""},
    {'m', 50, 0, 0, ""},
    {'n', 49, 0, 0, ""},
    {'o', 24, 0, 0, ""},
    {'p', 25, 0, 0, ""},
    {'q', 16, 0, 0, ""},
    {'r', 19, 0, 0, ""},
    {'s', 31, 0, 0, ""},
    {'t', 20, 0, 0, ""},
    {'u', 22, 0, 0, ""},
    {'v', 47, 0, 0, ""},
    {'w', 17, 0, 0, ""},
    {'x', 45, 0, 0, ""},
    {'y', 21, 0, 0, ""},
    {'z', 44, 0, 0, ""},
    {' ', 57, 0, 0, ""},
    {'_', 12, 0, 1, ""},
    {'=', 13, 0, 0, ""},
    {'?', 13, 0, 1, ""},
    {'/', 13, 0, 0, ""},
    {'1', 2, 0, 0, ""},
    {'2', 3, 0, 0, ""},
    {'3', 4, 0, 0, ""},
    {'4', 5, 0, 0, ""},
    {'5', 6, 0, 0, ""},
    {'6', 7, 0, 0, ""},
    {'7', 8, 0, 0, ""},
    {'8', 9, 0, 0, ""},
    {'9', 10, 0, 0, ""},
    {'0', 11, 0, 0, ""},
    {'-', 12, 0, 0, ""},
    {'+', 13, 0, 1, ""},
    {'<', 51, 0, 0, ""},
    {'>', 52, 0, 0, ""},
    {',', 51, 0, 1, ""},
    {'[', 26, 0, 0, ""},
    {']', 27, 0, 0, ""},
    {'1', 79, 0, 0, ""},
    {'2', 80, 0, 0, ""},
    {'3', 81, 0, 0, ""},
    {'4', 75, 0, 0, ""},
    {'5', 76, 0, 0, ""},
    {'6', 77, 0, 0, ""},
    {'7', 71, 0, 0, ""},
    {'8', 72, 0, 0, ""},
    {'9', 73, 0, 0, ""},
    {'0', 82, 0, 0, ""},
    {'.', 83, 0, 0, ""},
    {'\0', 0, 0, 0, ""}, //End Marker
};

#define MAX_KB_DEVICES 5
int keyboard[MAX_KB_DEVICES]; //Support up to 5 keyboard inputs
int kb_count;
int last_state;
int last_key;
int shift_mod;
int alt_mod;

void keyboard_init()
{
  kb_count = 0;
  char device[1024];
  shift_mod = 0;
  alt_mod = 0;

  DIR *dpdf;
  struct dirent *epdf;
  dpdf = opendir("/dev/input/by-id/");
  if (dpdf != NULL)
  {
    printf("Finding Keyboard devices!\n");
     while (epdf = readdir(dpdf))
     {
        //printf("Scanning keyboard inputs! %s\n", epdf->d_name);
        if (strstr(epdf->d_name, "kbd") != NULL)
        {
          sprintf(device, "/dev/input/by-id/%s", epdf->d_name);
          printf("\tFound Device: %s\n", device);
          if ((keyboard[kb_count] = open (device, O_RDONLY|O_NONBLOCK)) == -1)
          {
              printf ("\t\tCould not open keyboard device\n");
          }
          printf("\t\tOpened device!\n");
          kb_count++;
          if (kb_count == MAX_KB_DEVICES) break;
        }
     }
  }
  closedir(dpdf);
}
void keyboard_close(void)
{
  for(int x = 0; x < kb_count; x++)
  {
    close(keyboard[x]);
  }
}
void keyboard_tick(void)
{
  int rd;
  int value;
  int size = sizeof (struct input_event);
  int key = 0;
  struct input_event ev[1];
  for(int x = 0; x < kb_count; x++)
  {
    rd = read(keyboard[x], ev, size * 1);
    value = ev[0].value;
    key = ev[0].code;
  }
  if (key != 0 && value < 100)
  {
    keyboard_event(ev[0].code, ev[0].value);
  }
}
bool keyboard_read(lv_indev_data_t * data)
{
    data->state = (lv_indev_state_t)last_state;
    data->key = (lv_indev_state_t)last_key;

    return false;		/*No more data to read so return false*/
}
char keyboard_keymap_lookup(int keycode, int shift_mod, int alt_mod)
{
  int x = 0;
  do{
    if (keymap[x].keycode == keycode && keymap[x].shift_mod == shift_mod && keymap[x].alt_mod == alt_mod)
    {
      return keymap[x].letter;
    }
    x++;
  }while(keymap[x].letter != '\0');
}
void keyboard_event(int keycode, int state)
{
  int down = 1;
  int up = 0;
  int repeat = 2;

  //terminal_add_char(keycode);
  //printf ("keycode[%d], state[%d], shift_mod[%d], alt_mod[%d]\n", keycode, state, shift_mod, alt_mod);
  if (state == down)
  {
    //printf ("keycode[%d], state[%d], shift_mod[%d], alt_mod[%d]\n", keycode, state, shift_mod, alt_mod);
  }
  if (keycode == 42 && state == down)
  {
    shift_mod = state;
  }
  if (keycode == 42 && state == up)
  {
    shift_mod = state;
  }
  if (keycode == 56 && state == down)
  {
    alt_mod = state;
  }
  if (keycode == 56 && state == up)
  {
    alt_mod = state;
  }
  if (keycode == 1 && state == down) //Escape
  {
    linuxcnc_abort();
    //printf("Bye!\n");
    //kill_main();
    //return;
  }
  if (keycode == 59 && alt_mod == down && state == down) //Alt-F1
  {
    terminal_open();
  }
  if (keycode == 28 && alt_mod == up && shift_mod == up && state == down) //Enter
  {
    terminal_enter();
  }
  if (keycode == 14 && (state == down || state == repeat))
  {
    terminal_backspace();
  }
  if (keycode == 106) //Right Arrow
  {
    if  (state == down)
    {
      linuxcnc_jog_x_plus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_x_plus(false);
    }
  }
  if (keycode == 105) //Left Arrow
  {
    if  (state == down)
    {
      linuxcnc_jog_x_minus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_x_minus(false);
    }
  }
  if (keycode == 103) //Up Arrow
  {
    if  (state == down)
    {
      linuxcnc_jog_y_plus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_y_plus(false);
    }
  }
  if (keycode == 108) //Down Arrow
  {
    if  (state == down)
    {
      linuxcnc_jog_y_minus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_y_minus(false);
    }
  }
  if (keycode == 104) //Page Up
  {
    if  (state == down)
    {
      linuxcnc_jog_z_plus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_z_plus(false);
    }
  }
  if (keycode == 109) //Page Down
  {
    if  (state == down)
    {
      linuxcnc_jog_z_minus(true);
    }
    if (state == up)
    {
      linuxcnc_jog_z_minus(false);
    }
  }
  last_key = keyboard_keymap_lookup(keycode, shift_mod, alt_mod);
  //printf("Key is %c -> %d\n", last_key, state);
  last_state = state;
  if (state == down || state == repeat)
  {
    terminal_add_char(last_key);
  }
}
