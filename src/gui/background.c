#include "background.h"
#include "main.h"
#include "elements.h"

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
#include "javascript_vm/duktape.h"

void gui_set_background(int r, int g, int b)
{
  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;                                 /*Set a great horizontal padding*/
  style.body.main_color = LV_COLOR_MAKE(r, g, b);
  style.body.grad_color = LV_COLOR_MAKE(r, g, b);

  /*Create an other container*/
  lv_obj_t * box1;
  box1 = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(box1, &style);     /*Set the new style*/
  lv_cont_set_fit(box1, true, false); /*Do not enable the vertical fit */
  //lv_obj_set_height(box2, 55);        /*Set a fix height*/
  lv_obj_set_size(box1, 1680, 1050);
}
duk_ret_t javascript_gui_set_background(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  int r = duk_to_int(ctx, 0);
  int g = duk_to_int(ctx, 1);
  int b = duk_to_int(ctx, 2);
  gui_set_background(r, g, b);
	duk_push_number(ctx, 0);
	return 0;  /* one return value */
}
