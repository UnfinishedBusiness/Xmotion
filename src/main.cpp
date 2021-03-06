#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
//#include "lv_examples/lv_apps/demo/demo.h"
#include "config/handler.h"
#include "linuxcnc.h"
#include "input_devices/keyboard.h"
#include "input_devices/mouse.h"
#include "utils/hardware_utils.h"
#include "utils/duty_sim.h"
#include "gui/elements.h"
#include "javascript_vm/javascript.h"
#include "main.h"


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

#include <string>
#include <iostream>

using namespace std;

bool kill_main_flag = false;
void kill_main(void)
{
  kill_main_flag = true;
}
void delay(int millis)
{
  usleep(millis * 300);
}
int main(void)
{
    //gcode_parse("test/gcode_parser/3.ngc");
    //gcode_stack_dump();
    //return;
    DEBUG_PRINT(("Debug Mode!\n"));
    config_handler_init();
    /*LittlevGL init*/
    lv_init();

    mouse_init_();
    lv_indev_drv_t mouse_indev_drv;
    lv_indev_drv_init(&mouse_indev_drv);          /*Basic initialization*/
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;
    mouse_indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&mouse_indev_drv);

    keyboard_init();
    lv_indev_drv_t keyboard_indev_drv;
    lv_indev_drv_init(&keyboard_indev_drv);          /*Basic initialization*/
    keyboard_indev_drv.type = LV_INDEV_TYPE_POINTER;
    keyboard_indev_drv.read = keyboard_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * keyboard_indev = lv_indev_drv_register(&keyboard_indev_drv);

    LV_IMG_DECLARE(mouse_cursor);
    lv_obj_t * cursor_obj =  lv_img_create(lv_scr_act(), NULL); /*Create an image for the cursor */
    //lv_img_set_src(cursor_obj, SYMBOL_POWER);                 /*For simlicity add a built in symbol not an image*/
    lv_img_set_src(cursor_obj, &mouse_cursor);
    lv_indev_set_cursor(mouse_indev, cursor_obj); /* connect the object to the driver*/

    /*Linux frame buffer device init*/
    fbdev_init();
    duty_sim_init();

    /*Add a display the LittlevGL sing the frame buffer driver*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = fbdev_flush;      /*It flushes the internal graphical buffer to the frame buffer*/
    lv_disp_drv_register(&disp_drv);

    linuxcnc_init();

    hardware_utils_set_graphics_mode();

    javascript_modules_init();

    //gui_elements_nav_register_item("CNC", "plasma_ui();");

    //gui_plasma_control_ui_create();

    int nav_tick_timing = 0;
    int linuxcnc_poll_timing = 0;
    while(kill_main_flag == false)
    {
        lv_tick_inc(1);
        lv_task_handler(); //Should be called about every 5ms
        if (nav_tick_timing > 1000)
        {
          nav_tick_timing = 0;
          gui_elements_nav_tick();
          gui_elements_message_box_tick();
        }
        keyboard_tick();
        gui_elements_viewer_tick();
        mouse_tick();
        duty_sim_tick();
        if (linuxcnc_poll_timing > 100)
        {
          linuxcnc_poll_timing = 0;
          linuxcnc_tick();
          gui_elements_dro_tick();
          gui_elements_indicators_tick();
        }
        linuxcnc_poll_timing++;
        nav_tick_timing++;
    }
    javascript_modules_close();
    linuxcnc_close();
    //gui_plasma_control_ui_close(); //Need to call the last close item on the nav to free used memory
    hardware_utils_set_text_mode();
    mouse_close();
    keyboard_close();
    duty_sim_close();
    config_handler_close();

    return 0;
}
