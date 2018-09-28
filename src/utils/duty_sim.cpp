#include "duty_sim.h"
#include "gui/nc_viewer.h"
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

#include "gui/elements.h"

bool SIM;

void duty_sim_init(void)
{
  SIM = false;
}
void duty_sim_close(void)
{
  duty_sim_init(); //Re-init everything back to false
}
void duty_sim_dro(bool on)
{
  SIM = on;
}

float a = 0;
float b = 0;
float c = 0;
float d = 0;
float e = 0;
float f = 0;
bool one, two, three, four, five;
void duty_sim_tick(void)
{
  if (SIM)
  {
    a += 0.01;
    b += 0.02;
    c += 0.03;
    d += 0.04;
    e += 0.05;
    f += 0.06;
    if (a > 100)
    {
      a = 0;
      one = !one;
      gui_elements_indicators_set_floating_head_led(one);
    }
    if (b > 100)
    {
      b = 0;
      two = !two;
      gui_elements_indicators_set_torch_on_led(two);
    }
    if (c > 100)
    {
      c = 0;
      three = !three;
      gui_elements_indicators_set_arc_ok_led(three);
    }
    if (d > 100)
    {
      d = 0;
      four = !four;
      gui_elements_indicators_set_torch_up_led(four);
    }
    if (e > 100)
    {
      e = 0;
      five = !five;
      gui_elements_indicators_set_torch_down_led(five);
    }
    if (f > 100)
    {
      f = 0;
    }
    linuxcnc_position.mcs.x = a;
    linuxcnc_position.mcs.y = b;
    linuxcnc_position.mcs.z = c;
    linuxcnc_position.dro.x = a;
    linuxcnc_position.dro.y = b;
    linuxcnc_position.dro.z = c;
    gui_elements_viewer_set_redraw_flag();
    usleep(1000);
  }
}
