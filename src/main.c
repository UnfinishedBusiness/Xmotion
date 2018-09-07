#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_examples/lv_apps/demo/demo.h"

#include "input_devices/keyboard.h"
#include "input_devices/mouse.h"
#include "utils/hardware_utils.h"
#include "gui/jog_mode.h"


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

bool kill_main_flag = false;
void kill_main(void)
{
  kill_main_flag = true;
}

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    mouse_init_();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&indev_drv);

    LV_IMG_DECLARE(mouse_cursor);
    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL); /*Create an image for the cursor */
    //lv_img_set_src(cursor_obj, SYMBOL_POWER);                 /*For simlicity add a built in symbol not an image*/
    lv_img_set_src(cursor_obj, &mouse_cursor);
    lv_indev_set_cursor(mouse_indev, cursor_obj); /* connect the object to the driver*/


    /*Linux frame buffer device init*/
    fbdev_init();

    /*Add a display the LittlevGL sing the frame buffer driver*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = fbdev_flush;      /*It flushes the internal graphical buffer to the frame buffer*/
    lv_disp_drv_register(&disp_drv);

    hardware_utils_set_graphics_mode();
    keyboard_init();

    demo_create();
    //gui_jog_mode_create();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(kill_main_flag == false) {
        lv_tick_inc(100);
        lv_task_handler();
        keyboard_tick();
        mouse_tick();
        usleep(1000);
    }

    hardware_utils_set_text_mode();
    mouse_close();
    keyboard_close();

    return 0;
}
