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

#include <linuxcnc/emc.hh>
#include <linuxcnc/emc_nml.hh>

#include <iostream>
#include <cstdlib>
#include <cstring>


#include <python2.7/Python.h>

//https://github.com/LinuxCNC/linuxcnc/blob/0f91c553a238c3f5e8a52285044761c2dcfd7de5/src/emc/usr_intf/shcom.hh
//https://github.com/LinuxCNC/linuxcnc/blob/0f91c553a238c3f5e8a52285044761c2dcfd7de5/src/emc/usr_intf/shcom.cc

//http://www.linuxcnc.org/docs/html/config/python-interface.html#_usage_patterns_for_the_linuxcnc_nml_interface

linuxcnc_position_t linuxcnc_position;
RCS_STAT_CHANNEL *nml_stat;
EMC_STAT *emcStatus;
float jog_speed;

const char *nmlfile = "/usr/share/linuxcnc/linuxcnc.nml";

/* Private Functions */
bool poll_status()
{
  if(nml_stat->valid())
  {
    if(nml_stat->peek() == EMC_STAT_TYPE)
    {
      emcStatus = static_cast<EMC_STAT*>(nml_stat->get_address());
      return true;
    }
  }
  return false;
}
/* End Private Function */

void linuxcnc_init(void)
{
  nml_stat = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", nmlfile);
  linuxcnc_position.dro.x = 0;
  linuxcnc_position.dro.y = 0;
  linuxcnc_position.dro.z = 0;
  jog_speed = config.default_jog_speed / 60;
  Py_Initialize();
  PyRun_SimpleString("import linuxcnc\nc = linuxcnc.command()\nc.state(linuxcnc.STATE_ESTOP_RESET)\nc.state(linuxcnc.STATE_ON)\ns = linuxcnc.stat()\ne = linuxcnc.error_channel()");
}
void linuxcnc_close(void)
{
  nml_stat = NULL;
  Py_Finalize();
}
void wait_complete()
{
  char cmd[1024];
  sprintf(cmd, "c.wait_complete()\n");
  PyRun_SimpleString(cmd);
}
void jog_continous_plus(int axis)
{
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed);
  PyRun_SimpleString(cmd);
}
void jog_continous_minus(int axis)
{
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed * -1);
  PyRun_SimpleString(cmd);
}
void jog_stop(int axis)
{
  linuxcnc_jog_mode();
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
  jog_speed = speed / 60;
  //printf("Jog Speed [in/sec] = %0.4f\n", jog_speed);
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
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MANUAL)\n");
  PyRun_SimpleString(cmd);
  //wait_complete();
}
void linuxcnc_home_axis(int axis)
{
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
  char cmd[1024];
  sprintf(cmd, "c.abort()\n");
  PyRun_SimpleString(cmd);
}
void linuxcnc_program_open(char *file)
{
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
void linuxcnc_tick()
{
  if (poll_status())
  {
    linuxcnc_position.mcs.x = emcStatus->motion.traj.actualPosition.tran.x;
    linuxcnc_position.mcs.y = emcStatus->motion.traj.actualPosition.tran.y;
    linuxcnc_position.mcs.z = emcStatus->motion.traj.actualPosition.tran.z;

    linuxcnc_position.work_offset.x = emcStatus->task.g5x_offset.tran.x;
    linuxcnc_position.work_offset.y = emcStatus->task.g5x_offset.tran.y;
    linuxcnc_position.work_offset.z = emcStatus->task.g5x_offset.tran.z;

    linuxcnc_position.g92_offset.x = emcStatus->task.g92_offset.tran.x;
    linuxcnc_position.g92_offset.y = emcStatus->task.g92_offset.tran.y;
    linuxcnc_position.g92_offset.z = emcStatus->task.g92_offset.tran.z;

    linuxcnc_position.tool_offset.x = emcStatus->task.toolOffset.tran.x;
    linuxcnc_position.tool_offset.y = emcStatus->task.toolOffset.tran.y;
    linuxcnc_position.tool_offset.z = emcStatus->task.toolOffset.tran.z;


    linuxcnc_position.dro.x = linuxcnc_position.mcs.x - linuxcnc_position.work_offset.x - linuxcnc_position.g92_offset.x - linuxcnc_position.tool_offset.x;
    linuxcnc_position.dro.y = linuxcnc_position.mcs.y - linuxcnc_position.work_offset.y - linuxcnc_position.g92_offset.y - linuxcnc_position.tool_offset.y;
    linuxcnc_position.dro.z = linuxcnc_position.mcs.z - linuxcnc_position.work_offset.z - linuxcnc_position.g92_offset.z - linuxcnc_position.tool_offset.z;
  }

}
