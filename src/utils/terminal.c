#include "terminal.h"

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

#include "utils/duty_sim.h"
#include "gui/elements.h"

#define TERMINAL_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define TERMINAL_TEXT_COLOR LV_COLOR_MAKE(255, 255, 255);
lv_obj_t *terminal_container;
lv_obj_t *cmd_line;
lv_obj_t *cmd_line_output;
char cmd_string[2048];

void terminal_add_char(char c)
{
  if (terminal_container != NULL)
  {
    sprintf(cmd_string, "%s%c", cmd_string, c);
    lv_label_set_text(cmd_line, cmd_string);
  }
}
void terminal_backspace(void)
{
  if (terminal_container != NULL)
  {
    cmd_string[strlen(cmd_string) - 1] = '\0';
    lv_label_set_text(cmd_line, cmd_string);
  }
}
void terminal_enter(void)
{
  terminal_eval(cmd_string);
}
void terminal_open(void)
{
  static lv_style_t style_text_input;
  lv_style_copy(&style_text_input, &lv_style_pretty);
  style_text_input.body.shadow.width = 8;
  style_text_input.text.color = LV_COLOR_MAKE(255, 255, 255);
  style_text_input.text.font = &lv_font_dejavu_40;

  static lv_style_t small_text_style;
  lv_style_copy(&small_text_style, &lv_style_plain);
  small_text_style.text.color = TERMINAL_TEXT_COLOR;
  small_text_style.text.font = &lv_font_dejavu_10;

  static lv_style_t text_style;
  lv_style_copy(&text_style, &lv_style_plain);
  text_style.text.color = TERMINAL_TEXT_COLOR;
  text_style.text.font = &lv_font_dejavu_40;

  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 6;
  style.body.padding.hor = 5;
  style.body.main_color = TERMINAL_BACKGROUND_COLOR;
  style.body.grad_color = TERMINAL_BACKGROUND_COLOR;

  /*Create an other container*/
  terminal_container = lv_cont_create(lv_scr_act(), NULL);
  lv_obj_set_style(terminal_container, &style);     /*Set the new style*/
  //lv_cont_set_fit(text_container, true, false); /*Do not enable the vertical fit */
  lv_obj_set_size(terminal_container, 800, 600);
  lv_obj_align(terminal_container, NULL, LV_ALIGN_IN_TOP_MID, 0, 100);

  lv_obj_t *ps1 = lv_label_create(terminal_container, NULL);
  lv_obj_set_style(ps1, &text_style);
  lv_label_set_text(ps1, "=>");
  lv_obj_align(ps1, NULL, LV_ALIGN_IN_TOP_LEFT, 30, 10);

  cmd_line = lv_label_create(terminal_container, NULL);
  lv_obj_set_style(cmd_line, &text_style);
  lv_label_set_text(cmd_line, "");
  lv_obj_align(cmd_line, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 10);

  cmd_line_output = lv_label_create(terminal_container, NULL);
  lv_obj_set_style(cmd_line_output, &text_style);
  lv_label_set_text(cmd_line_output, "");
  lv_obj_align(cmd_line_output, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 100);

}
void terminal_close(void)
{
  if (terminal_container != NULL)
  {
    lv_obj_del(terminal_container);
    terminal_container = NULL;
  }
}
void terminal_eval(char cmd[2048])
{
  //lv_label_set_text(cmd_line, "");
  if (!strcmp(cmd, "sim_dro=true"))
  {
    duty_sim_dro(true);
    lv_label_set_text(cmd_line_output, "SIM on!");
    return;
  }
  if (!strcmp(cmd, "sim_dro=false"))
  {
    duty_sim_dro(false);
    lv_label_set_text(cmd_line_output, "SIM off!");
    return;
  }
  if (!strcmp(cmd, "dro=true"))
  {
    gui_elements_dro();
    lv_label_set_text(cmd_line_output, "DRO is open!");
    return;
  }
  if (!strcmp(cmd, "dro=false"))
  {
    gui_elements_dro_close();
    lv_label_set_text(cmd_line_output, "DRO is closed!");
    return;
  }
  if (!strcmp(cmd, "close"))
  {
    printf("Closing terminal!\n");
    terminal_close();
    return;
  }
}
