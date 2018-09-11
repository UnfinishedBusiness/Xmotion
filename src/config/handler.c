#include "handler.h"
#include "ini.h"


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

configuration config;

static int config_handler(void* conf, const char* section, const char* name, const char* value)
{
    configuration* pconfig = (configuration*)conf;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("system", "screen_size"))
    {
        pconfig->screen_size = strdup(value);
    }
    else if (MATCH("system", "screen_color"))
    {
        pconfig->screen_color = strdup(value);
    }
    else if (MATCH("system", "keyboard_device"))
    {
        pconfig->keyboard_device = strdup(value);
    }
    else if (MATCH("system", "mouse_device"))
    {
        pconfig->mouse_device = strdup(value);
    }
    else if (MATCH("motion", "max_jog_speed"))
    {
        pconfig->max_jog_speed = atof(value);
    }
    else if (MATCH("motion", "max_jog_speed"))
    {
        pconfig->default_jog_speed = atof(value);
    }
    else
    {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}
void config_handler_set_defaults()
{
  char *kb = "/dev/input/event0";
  char *ms = "/dev/input/mice";
  config.keyboard_device = kb;
  config.mouse_device = ms;

  config.max_jog_speed = 600;
  config.default_jog_speed = 400;
}
void config_handler_init(void)
{
  config_handler_set_defaults();
  if (ini_parse("/etc/Xmotion/config.ini", config_handler, &config) < 0) {
      printf("Can't load '/etc/Xmotion/config.ini'\n");
      return;
  }
  printf("Config loaded from '/etc/Xmotion/config.ini'\n");
}
void config_handler_close(void)
{

}
