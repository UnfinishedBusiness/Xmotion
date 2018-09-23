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

int tty;

void hardware_utils_set_graphics_mode(void)
{
  //system("setterm -powersave off -blank 0");
  tty = open("/dev/tty0", O_RDWR);
  if(ioctl(tty, KDSETMODE, KD_GRAPHICS) == -1)
  {
    printf("Failed to set graphics mode on tty0\n");
  }
  close(tty);

  tty = open("/dev/tty0", O_RDWR);
  if(ioctl(tty, KDSKBMODE, K_OFF) == -1)
  {
    printf("Failed to set keyboard mute mode\n");
  }
  close(tty);
}
void hardware_utils_set_text_mode(void)
{
  tty = open("/dev/tty0", O_RDWR);
  if(ioctl(tty, KDSKBMODE, K_UNICODE) == -1)
  {
    printf("Failed to un-mute keyboard\n");
  }
  close(tty);

  tty = open("/dev/tty0", O_RDWR);
  if(ioctl(tty, KDSETMODE, KD_TEXT) == -1)
  {
    printf("Failed to set text mode on tty0\n");
  }
  close(tty);
}
void hardware_utils_print_screensize(void)
{
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;


  int fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1)
  {
      perror("Error: cannot open framebuffer device");
      exit(1);
  }
   // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
      perror("Error reading fixed information");
      exit(2);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
      perror("Error reading variable information");
      exit(3);
  }
  //printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  screensize = vinfo.yres_virtual * finfo.line_length;
  //printf("screensize: %d\n",screensize);
  close(fbfd);
}
