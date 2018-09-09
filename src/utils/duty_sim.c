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

bool DRO;

void duty_sim_init(void)
{
  DRO = false;
}
void duty_sim_close(void)
{
  duty_sim_init(); //Re-init everything back to false
}
void duty_sim_dro(bool on)
{
  DRO = on;
}

float a = 0;
float b = 0;
float c = 0;
float d = 0;
float e = 0;
float f = 0;
void duty_sim_tick(void)
{
  if (DRO)
  {
    a += 0.1;
    b += 0.2;
    c += 0.3;
    d += 0.4;
    e += 0.5;
    f += 0.6;
    if (a > 1000) a = 0;
    if (b > 1000) b = 0;
    if (c > 1000) c = 0;
    if (d > 1000) d = 0;
    if (e > 1000) e = 0;
    if (f > 1000) f = 0;
    gui_elements_dro_update_wcs(a, b, c);
    gui_elements_dro_update_abs(d, e, f);
  }
}
