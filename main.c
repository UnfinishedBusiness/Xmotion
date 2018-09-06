#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_examples/lv_apps/demo/demo.h"


//https://gist.github.com/robmint/4753401


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

int main(void)
{
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    int keyboard;
    int rd;
    int value;
    int size = sizeof (struct input_event);
    int key = 0;
    struct input_event ev[64];
  
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");
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
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    screensize = vinfo.yres_virtual * finfo.line_length;
    printf("screensize: %d\n",screensize); 
    close(fbfd);
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*Add a display the LittlevGL sing the frame buffer driver*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = fbdev_flush;      /*It flushes the internal graphical buffer to the frame buffer*/
    lv_disp_drv_register(&disp_drv);


    int tty = open("/dev/tty1", O_RDWR);
    if(ioctl(tty, KDSETMODE, KD_GRAPHICS) == -1)
    {
      printf("Failed to set graphics mode on tty1\n");
    }
	    

    
    /*if ((keyboard = open ("/dev/input/event1", O_RDONLY|O_NONBLOCK)) == -1) {
        printf ("Could not open keyboard device\n");
    }*/

    demo_create();


    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_tick_inc(5);
        lv_task_handler();

	      /*rd = read(keyboard, ev, size * 64);
	      value = ev[0].value;
        if (value != ' ' && ev[1].type == 1){ // Only read the key press event
		      key = ev[1].code;
       		printf ("Code[%d]\n", (ev[1].code));
          printf ("Value[%d]\n", (ev[1].value));
      	}
        if (ev[1].code == 1) //Escape
        {
          ioctl(tty, KDSETMODE, KD_TEXT);
          return 0;
        }
        if (ev[1].code == 106) //Right Arrow
        {
          tab_num++;
          if (tab_num > 3) tab_num = 0;
          if (tab_num = 0)
          {
            lv_tabview_set_tab_act(tv, tab1, true);
          }
          if (tab_num = 1)
          {
            lv_tabview_set_tab_act(tv, tab2, true);
          }
          if (tab_num = 2)
          {
            lv_tabview_set_tab_act(tv, tab3, true);
          }
          
        }
        if (ev[1].code == 105) //Left Arrow
          {
            tab_num--;
            if (tab_num < 0) tab_num = 3;
            if (tab_num = 0)
            {
              lv_tabview_set_tab_act(tv, tab1, true);
            }
            if (tab_num = 1)
            {
              lv_tabview_set_tab_act(tv, tab2, true);
            }
            if (tab_num = 2)
            {
              lv_tabview_set_tab_act(tv, tab3, true);
            }

          }*/

        usleep(5000);
    }

    return 0;
}
