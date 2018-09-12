#include "linuxcnc.h"
#include "config/handler.h"

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


#include <python2.7/Python.h>

float jog_speed;

void linuxcnc_init(void)
{
  jog_speed = config.default_jog_speed / 60;
  Py_Initialize();
  PyRun_SimpleString("import linuxcnc\nc = linuxcnc.command()\nc.state(linuxcnc.STATE_ESTOP_RESET)\nc.state(linuxcnc.STATE_ON)\n");
}
void linuxcnc_close(void)
{
  Py_Finalize();
}
void jog_continous_plus(int axis)
{
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed);
  PyRun_SimpleString(cmd);
}
void jog_continous_minus(int axis)
{
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed * -1);
  PyRun_SimpleString(cmd);
}
void jog_stop(int axis)
{
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_STOP, %d)\n", axis);
  PyRun_SimpleString(cmd);
}
void linuxcnc_jog_x_plus(bool jog)
{
  if (jog == true)
  {
    jog_continous_plus(0);
  }
  else
  {
    jog_stop(0);
  }
}
void linuxcnc_jog_x_minus(bool jog)
{
  if (jog == true)
  {
    jog_continous_minus(0);
  }
  else
  {
    jog_stop(0);
  }
}
void linuxcnc_jog_y_plus(bool jog)
{
  if (jog == true)
  {
    jog_continous_plus(1);
  }
  else
  {
    jog_stop(1);
  }
}
void linuxcnc_jog_y_minus(bool jog)
{
  if (jog == true)
  {
    jog_continous_minus(1);
  }
  else
  {
    jog_stop(1);
  }
}
void linuxcnc_jog_z_plus(bool jog)
{
  if (jog == true)
  {
    jog_continous_plus(2);
  }
  else
  {
    jog_stop(2);
  }
}
void linuxcnc_jog_z_minus(bool jog)
{
  if (jog == true)
  {
    jog_continous_minus(2);
  }
  else
  {
    jog_stop(2);
  }
}
void linuxcnc_set_jog_speed(float speed)
{
  /*char cmd[1024];
  sprintf(cmd, "halcmd setp halui.jog-speed %0.4f\n", speed);
  system(cmd);*/
  jog_speed = speed / 60;
  printf("Jog Speed [in/sec] = %0.4f\n", jog_speed);
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

float linuxcnc_get_pin_state(char *pin)
{
  char cmd[1024];
  sprintf(cmd, "halcmd getp %s", pin);
  FILE *cmd_p = popen(cmd, "r");
  if (!cmd_p)
  {
    return false;
  }
  char buffer[1024];
  char *line_p = fgets(buffer, sizeof(buffer), cmd_p);
  pclose(cmd_p);
  line_p[strlen(line_p) - 1] = '\0';
  if (strcmp(line_p, "TRUE") != 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}
