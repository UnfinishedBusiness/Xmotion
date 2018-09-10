#include "cnc_control.h"
#include "elements.h"
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

void gui_cnc_control_create()
{
  gui_elements_background();
  gui_elements_dro();
  gui_elements_controls();
  gui_elements_indicators();
  gui_elements_nav();
  gui_elements_viewer();
}
void gui_cnc_control_close()
{
  gui_elements_dro_close();
  gui_elements_controls_close();
  gui_elements_indicators_close();
  gui_elements_nav_close();
  gui_elements_viewer_close();
}
