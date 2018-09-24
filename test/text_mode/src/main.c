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
#include <unistd.h>


int main(int argc, char **argv)
{
  int tty = open("/dev/tty0", O_RDWR);
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
