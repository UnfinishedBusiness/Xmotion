#include "duty_sim.h"

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
    a += 1.1;
    b += 1.2;
    c += 1.3;
    d += 1.4;
    e += 1.5;
    f += 1.6;
    if (a > 1000)
    {
      a = 0;
      one = !one;
      gui_elements_indicators_set_floating_head_led(one);
    }
    if (b > 1000)
    {
      b = 0;
      two = !two;
      gui_elements_indicators_set_torch_on_led(two);
    }
    if (c > 1000)
    {
      c = 0;
      three = !three;
      gui_elements_indicators_set_arc_ok_led(three);
    }
    if (d > 1000)
    {
      d = 0;
      four = !four;
      gui_elements_indicators_set_torch_up_led(four);
    }
    if (e > 1000)
    {
      e = 0;
      five = !five;
      gui_elements_indicators_set_torch_down_led(five);
    }
    if (f > 1000)
    {
      f = 0;
    }
    gui_elements_dro_update_wcs(a, b, c);
    gui_elements_dro_update_abs(d, e, f);
  }
}
