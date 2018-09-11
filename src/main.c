#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
//#include "lv_examples/lv_apps/demo/demo.h"
#include "config/ini.h"
#include "input_devices/keyboard.h"
#include "input_devices/mouse.h"
#include "utils/hardware_utils.h"
#include "utils/duty_sim.h"
#include "gui/elements.h"
#include "gui/cnc_control.h"


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

typedef struct
{
    const char* screen_size;
    const char* screen_color;
    const char* keyboard_device;
    const char* mouse_device;
} configuration;

static int config_handler(void* conf, const char* section, const char* name, const char* value)
{
    configuration* pconfig = (configuration*)conf;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("system", "screen_size")) {
        pconfig->screen_size = atoi(value);
    } else if (MATCH("system", "screen_color")) {
        pconfig->screen_color = strdup(value);
    } else if (MATCH("system", "keyboard_device")) {
        pconfig->keyboard_device = strdup(value);
    } else if (MATCH("system", "mouse_device")) {
        pconfig->mouse_device = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int main(void)
{

    configuration config;

    if (ini_parse("/etc/Xmotion/config.ini", config_handler, &config) < 0) {
        printf("Can't load '/etc/Xmotion/config.ini'\n");
        return 1;
    }
    printf("Config loaded from '/etc/Xmotion/config.ini'\n");
    /*LittlevGL init*/
    lv_init();

    mouse_init_(config.mouse_device);
    lv_indev_drv_t mouse_indev_drv;
    lv_indev_drv_init(&mouse_indev_drv);          /*Basic initialization*/
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;
    mouse_indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_t * mouse_indev = lv_indev_drv_register(&mouse_indev_drv);

    keyboard_init(config.keyboard_device);
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

    hardware_utils_set_graphics_mode();

    gui_cnc_control_create();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(kill_main_flag == false) {
        lv_tick_inc(100);
        lv_task_handler();
        keyboard_tick();
        mouse_tick();
        duty_sim_tick();
        usleep(1000);
    }
    gui_cnc_control_close();
    hardware_utils_set_text_mode();
    mouse_close();
    keyboard_close();
    duty_sim_close();

    return 0;
}
