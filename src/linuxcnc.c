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
float linuxcnc_get_x_rel_position(void)
{
  FILE *cmd_p = popen("halcmd getp halui.axis.0.pos-relative", "r");
  if (!cmd_p)
  {
    return -1000;
  }
  char buffer[1024];
  char *line_p = fgets(buffer, sizeof(buffer), cmd_p);
  pclose(cmd_p);
  line_p[strlen(line_p) - 1] = '\0';
  return atof(line_p);
}
float linuxcnc_get_y_rel_position(void)
{
  FILE *cmd_p = popen("halcmd getp halui.axis.1.pos-relative", "r");
  if (!cmd_p)
  {
    return -1000;
  }
  char buffer[1024];
  char *line_p = fgets(buffer, sizeof(buffer), cmd_p);
  pclose(cmd_p);
  line_p[strlen(line_p) - 1] = '\0';
  return atof(line_p);
}
float linuxcnc_get_z_rel_position(void)
{
  FILE *cmd_p = popen("halcmd getp halui.axis.2.pos-relative", "r");
  if (!cmd_p)
  {
    return -1000;
  }
  char buffer[1024];
  char *line_p = fgets(buffer, sizeof(buffer), cmd_p);
  pclose(cmd_p);
  line_p[strlen(line_p) - 1] = '\0';
  return atof(line_p);
}
