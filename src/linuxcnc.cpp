#include "linuxcnc.h"
#include "config/handler.h"
#include "gui/nc_viewer.h"
#include "gui/nc_dro.h"
#include "gui/nc_message_box.h"

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

#ifndef SIM_MODE

#endif

#include <iostream>
#include <cstdlib>
#include <cstring>


#include <python2.7/Python.h>

//https://github.com/LinuxCNC/linuxcnc/blob/0f91c553a238c3f5e8a52285044761c2dcfd7de5/src/emc/usr_intf/shcom.hh
//https://github.com/LinuxCNC/linuxcnc/blob/0f91c553a238c3f5e8a52285044761c2dcfd7de5/src/emc/usr_intf/shcom.cc

//http://www.linuxcnc.org/docs/html/config/python-interface.html#_usage_patterns_for_the_linuxcnc_nml_interface

/*
The final goal is to make all linuxcnc interaction via NML done via the native C/C++ NML interface. Right now some interactions are done this way but due to a bunch of changes between 2.6 and 2.7 we don't
want to beak backwards compatibility support by hardcoding NML messages that are different between 2.6 and 2.7 (like the jog interface for example). So we will continue to use the python interface until we figure
out the best way to use the NML withouth breaking backwards compatibility. - Travis Gillin (9/24/18)
*/
#ifdef SIM_MODE //Defined during "make sim"
  #define _SIM 1
#else
  #define _SIM 0
#endif


#define SIM_BREAK if (_SIM == 1) return

#define EMC_COMMAND_DELAY   0.1	// how long to sleep between checks

linuxcnc_position_t linuxcnc_position;
float jog_speed;

/* Private Functions */

bool poll_error()
{
  return false;
}
bool poll_status()
{
 return false;
}
/* End Private Function */

void linuxcnc_init(void)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  emcStatusBuffer = new RCS_STAT_CHANNEL(emcFormat, "emcStatus", "xemc", emc_nmlfile);
  emcCommandBuffer = new RCS_CMD_CHANNEL(emcFormat, "emcCommand", "xemc", emc_nmlfile);
  emcErrorBuffer = new NML(nmlErrorFormat, "emcError", "xemc", emc_nmlfile);


  linuxcnc_position.dro.x = 0;
  linuxcnc_position.dro.y = 0;
  linuxcnc_position.dro.z = 0;
  jog_speed = config.default_jog_speed / 60;
  Py_Initialize();
  PyRun_SimpleString("import linuxcnc\nc = linuxcnc.command()\nc.state(linuxcnc.STATE_ESTOP_RESET)\nc.state(linuxcnc.STATE_ON)\ns = linuxcnc.stat()\ne = linuxcnc.error_channel()");
  #endif
}
void linuxcnc_close(void)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  emcStatusBuffer = NULL;
  Py_Finalize();
  #endif
}
void wait_complete()
{
  SIM_BREAK;
  #ifndef SIM_MODE
  char cmd[1024];
  sprintf(cmd, "c.wait_complete()\n");
  PyRun_SimpleString(cmd);
  //emcCommandWaitDone();
  #endif
}
void jog_continous_plus(int axis)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed);
  PyRun_SimpleString(cmd);
  #endif
}
void jog_continous_minus(int axis)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_CONTINUOUS, %d, %0.4f)\n", axis, jog_speed * -1);
  PyRun_SimpleString(cmd);
  #endif
}
void jog_stop(int axis)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_jog_mode();
  char cmd[1024];
  sprintf(cmd, "c.jog(linuxcnc.JOG_STOP, %d)\n", axis);
  PyRun_SimpleString(cmd);
  #endif
}
void linuxcnc_jog_x_plus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_plus(0);
  }
  else
  {
    jog_stop(0);
  }
  #endif
}
void linuxcnc_jog_x_minus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_minus(0);
  }
  else
  {
    jog_stop(0);
  }
  #endif
}
void linuxcnc_jog_y_plus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_plus(1);
  }
  else
  {
    jog_stop(1);
  }
  #endif
}
void linuxcnc_jog_y_minus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_minus(1);
  }
  else
  {
    jog_stop(1);
  }
  #endif
}
void linuxcnc_jog_z_plus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_plus(2);
  }
  else
  {
    jog_stop(2);
  }
  #endif
}
void linuxcnc_jog_z_minus(bool jog)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (jog == true)
  {
    jog_continous_minus(2);
  }
  else
  {
    jog_stop(2);
  }
  #endif
}
void linuxcnc_set_jog_speed(float speed)
{
  SIM_BREAK;
  jog_speed = speed / 60;
  //printf("Jog Speed [in/sec] = %0.4f\n", jog_speed);
}
float linuxcnc_get_pin_state(char *pin)
{
  SIM_BREAK true;
  #ifndef SIM_MODE
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
  #endif
}
bool linuxcnc_is_axis_homed(int axis)
{
  SIM_BREAK false;
  #ifndef SIM_MODE
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
  #endif
}
void linuxcnc_jog_mode()
{
  SIM_BREAK;
  #ifndef SIM_MODE
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MANUAL)\n");
  PyRun_SimpleString(cmd);
  //wait_complete();
  gui_elements_dro_set_machine_state_indicator(DRO_JOG_MODE);
  #endif
}
void linuxcnc_auto_mode()
{
  SIM_BREAK;
  #ifndef SIM_MODE
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_AUTO)\n");
  PyRun_SimpleString(cmd);
  //wait_complete();
  gui_elements_dro_set_machine_state_indicator(DRO_AUTO_MODE);
  #endif
}
void linuxcnc_mdi_mode()
{
  SIM_BREAK;
  #ifndef SIM_MODE
  char cmd[1024];
  sprintf(cmd, "c.mode(linuxcnc.MODE_MDI)\n");
  PyRun_SimpleString(cmd);
  //wait_complete();
  gui_elements_dro_set_machine_state_indicator(DRO_MDI_MODE);
  #endif
}
void linuxcnc_home_axis(int axis)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_jog_mode();
  wait_complete();
  char cmd[1024];
  sprintf(cmd, "c.home(%d)\n", axis);
  PyRun_SimpleString(cmd);
  wait_complete();
  #endif
}
void linuxcnc_unhome_axis(int axis)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_jog_mode();
  wait_complete();
  char cmd[1024];
  sprintf(cmd, "c.unhome(%d)\n", axis);
  PyRun_SimpleString(cmd);
  wait_complete();
  #endif
}
void linuxcnc_mdi(char *mdi)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_mdi_mode();
  wait_complete();
  char cmd[1024];
  sprintf(cmd, "c.mdi(\"%s\")\n", mdi);
  PyRun_SimpleString(cmd);
  //wait_complete();
  //sendMdiCmd(mdi);
  #endif
}
void linuxcnc_abort(void)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  char cmd[1024];
  sprintf(cmd, "c.abort()\n");
  PyRun_SimpleString(cmd);
  gui_elements_dro_set_machine_state_indicator(DRO_ABORT_MODE);
  #endif
}
void linuxcnc_program_open(const char *file)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_auto_mode();
  wait_complete();
  char cmd[1024];
  sprintf(cmd, "c.reset_interpreter()\n");
  PyRun_SimpleString(cmd);
  printf("linuxcnc_program_open: %s\n", file);
  sprintf(cmd, "c.program_open(\"%s\")\n", file);
  PyRun_SimpleString(cmd);
  wait_complete();
  #endif
}
void linuxcnc_cycle_start(int start_line)
{
  SIM_BREAK;
  #ifndef SIM_MODE
  linuxcnc_auto_mode();
  char cmd[1024];
  sprintf(cmd, "c.auto(linuxcnc.AUTO_RUN, %d)\n", start_line);
  PyRun_SimpleString(cmd);
  #endif
}
void linuxcnc_tick()
{
  SIM_BREAK;
  #ifndef SIM_MODE
  if (poll_error())
  {
    printf("operator_text_string: %s\n", operator_text_string);
    printf("operator_display_string: %s\n", operator_display_string);
    printf("error_string: %s\n", error_string);
    if (strlen(error_string) > 1)
    {
      gui_elements_message_box_push(800, 60, error_string, 10, 10, 1);
    }

  }
  if (poll_status())
  {
    if (linuxcnc_position.mcs.x != emcStatus->motion.traj.actualPosition.tran.x || linuxcnc_position.mcs.y != emcStatus->motion.traj.actualPosition.tran.y || linuxcnc_position.mcs.z != emcStatus->motion.traj.actualPosition.tran.z)
    {
      gui_elements_viewer_set_redraw_flag();
    }
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
  #endif
}
