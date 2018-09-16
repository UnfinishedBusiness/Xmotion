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

//#include <linuxcnc/emc.hh>
//#include <linuxcnc/emc_nml.hh>

#include <iostream>
#include <cstdlib>
#include <cstring>


#include <python2.7/Python.h>

//http://www.linuxcnc.org/docs/html/config/python-interface.html#_usage_patterns_for_the_linuxcnc_nml_interface

//RCS_STAT_CHANNEL *nml_stat;
float jog_speed;
float linuxcnc_x_dro;
float linuxcnc_y_dro;

const char *nmlfile = "/usr/share/linuxcnc/linuxcnc.nml";

void linuxcnc_init(void)
{
  //nml_stat = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", nmlfile);
  linuxcnc_x_dro = 0;
  linuxcnc_y_dro = 0;
  jog_speed = config.default_jog_speed / 60;
  Py_Initialize();
  PyRun_SimpleString("import linuxcnc\nc = linuxcnc.command()\nc.state(linuxcnc.STATE_ESTOP_RESET)\nc.state(linuxcnc.STATE_ON)\ns = linuxcnc.stat()\ne = linuxcnc.error_channel()");
}
void linuxcnc_close(void)
{
  //nml_stat = NULL;
  Py_Finalize();
}
void wait_complete()
{
  return;
  char cmd[1024];
  sprintf(cmd, "c.wait_complete()\n");
  PyRun_SimpleString(cmd);
}
void jog_continous_plus(int axis)
{
  return;
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed);
  PyRun_SimpleString(cmd);
}
void jog_continous_minus(int axis)
{
  return;
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed * -1);
  PyRun_SimpleString(cmd);
}
void jog_stop(int axis)
{
  return;
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_STOP, %d)\n", axis);
  PyRun_SimpleString(cmd);
}
void linuxcnc_jog_x_plus(bool jog)
{
 return;
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
return;
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
return;
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
return;
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
return;
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
return;
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
  jog_speed = speed / 60;
  //printf("Jog Speed [in/sec] = %0.4f\n", jog_speed);
}
float linuxcnc_get_x_rel_position(void)
{
  /*while(1)
  {
    if(!nml_stat->valid()) continue;
    if(nml_stat->peek() != EMC_STAT_TYPE) continue;
    EMC_STAT *emcStatus = static_cast<EMC_STAT*>(nml_stat->get_address());
    //printf("X location = %0.4f\n", emcStatus->motion.traj.position.tran.x);
    linuxcnc_x_dro = emcStatus->motion.traj.position.tran.x;
    return linuxcnc_x_dro;
  }*/
  return -1000;
}
float linuxcnc_get_y_rel_position(void)
{
  /*while(1)
  {
    if(!nml_stat->valid()) continue;
    if(nml_stat->peek() != EMC_STAT_TYPE) continue;
    EMC_STAT *emcStatus = static_cast<EMC_STAT*>(nml_stat->get_address());
    //printf("X location = %0.4f\n", emcStatus->motion.traj.position.tran.x);
    linuxcnc_y_dro = emcStatus->motion.traj.position.tran.y;
    return emcStatus->motion.traj.position.tran.y;
  }*/
  return -1000;
}
float linuxcnc_get_z_rel_position(void)
{
  /*while(1)
  {
    if(!nml_stat->valid()) continue;
    if(nml_stat->peek() != EMC_STAT_TYPE) continue;
    EMC_STAT *emcStatus = static_cast<EMC_STAT*>(nml_stat->get_address());
    //printf("X location = %0.4f\n", emcStatus->motion.traj.position.tran.x);
    return emcStatus->motion.traj.position.tran.z;
  }*/
  return -1000;
}

float linuxcnc_get_pin_state(char *pin)
{
return false;
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
  //printf("GETP Says: %s\n", line_p);
  if (strcmp(line_p, "TRUE") != 0)
  {
    //printf("\tReturn False!\n");
    return false;
  }
  else
  {
    //printf("\tReturn True!\n");
    return true;
  }
}
bool linuxcnc_is_axis_homed(int axis)
{
return false;
  usleep(5 * 100000); //Wait two seconds
  char cmd[1024];
  sprintf(cmd, "halcmd getp halui.joint.%d.is-homed", axis);
  FILE *cmd_p = popen(cmd, "r");
  if (!cmd_p)
  {
    return false;
  }
  char buffer[1024];
  char *line_p = fgets(buffer, sizeof(buffer), cmd_p);
  pclose(cmd_p);
  line_p[strlen(line_p) - 1] = '\0';
  printf("GETP Says: %s\n", line_p);
  if (strcmp(line_p, "TRUE") != 0)
  {
    printf("\tReturn False!\n");
    return false;
  }
  else
  {
    printf("\tReturn True!\n");
    return true;
  }
}
void linuxcnc_jog_mode()
{
return;
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MANUAL)\n");
  PyRun_SimpleString(cmd);
  //wait_complete();
}
void linuxcnc_home_axis(int axis)
{
return;
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MANUAL)\n");
  PyRun_SimpleString(cmd);
  wait_complete();
  sprintf(cmd, "c.home(%d)\n", axis);
  PyRun_SimpleString(cmd);
  wait_complete();
}
void linuxcnc_unhome_axis(int axis)
{
return;
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MANUAL)\n");
  PyRun_SimpleString(cmd);
  wait_complete();
  sprintf(cmd, "c.unhome(%d)\n", axis);
  PyRun_SimpleString(cmd);
  wait_complete();
}
void linuxcnc_mdi(char *mdi)
{
return;
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MDI)\n");
  PyRun_SimpleString(cmd);
  wait_complete();
  sprintf(cmd, "c.mdi(\"%s\")\n", mdi);
  PyRun_SimpleString(cmd);
  //wait_complete();
}
void linuxcnc_abort(void)
{
return;
  char cmd[1024];
  sprintf(cmd, "c.abort()\n");
  PyRun_SimpleString(cmd);
}
void linuxcnc_program_open(char *file)
{
return;
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_AUTO)\n");
  PyRun_SimpleString(cmd);
  wait_complete();
  sprintf(cmd, "c.program_open(\"%s\")\n", file);
  PyRun_SimpleString(cmd);
  wait_complete();
  sprintf(cmd, "c.reset_interpreter()\n");
  PyRun_SimpleString(cmd);

}
