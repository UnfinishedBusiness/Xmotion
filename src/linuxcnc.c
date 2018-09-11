#include "linuxcnc.h"


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

void linuxcnc_init(void)
{

}
void linuxcnc_close(void)
{

}
void linuxcnc_jog_x_plus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.0.plus true");
  }
  else
  {
    system("halcmd setp halui.jog.0.plus false");
  }
}
void linuxcnc_jog_x_minus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.0.minus true");
  }
  else
  {
    system("halcmd setp halui.jog.0.minus false");
  }
}
void linuxcnc_jog_y_plus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.1.plus true");
  }
  else
  {
    system("halcmd setp halui.jog.1.plus false");
  }
}
void linuxcnc_jog_y_minus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.1.minus true");
  }
  else
  {
    system("halcmd setp halui.jog.1.minus false");
  }
}
void linuxcnc_jog_z_plus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.2.plus true");
  }
  else
  {
    system("halcmd setp halui.jog.2.plus false");
  }
}
void linuxcnc_jog_z_minus(bool jog)
{
  if (jog == true)
  {
    system("halcmd setp halui.jog.2.minus true");
  }
  else
  {
    system("halcmd setp halui.jog.2.minus false");
  }
}
void linuxcnc_set_jog_speed(float speed)
{
  char cmd[1024];
  sprintf(cmd, "halcmd setp halui.jog-speed %0.4f\n", speed);
  system(cmd);
}
