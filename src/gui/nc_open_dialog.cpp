#include "elements.h"
#include "linuxcnc.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "utils/gcode_parser.h"
#include "geometry/geometry.h"
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
#include <dirent.h>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;

/*vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}*/

typedef struct
{
  std::string name;
  std::string path;
}gcode_file_t;

typedef struct
{
  lv_obj_t *container;
  lv_obj_t *image;
  lv_obj_t *button;
  lv_obj_t *label;
  float x_offset;
  float y_offset;
  std::string name;
  std::string path;
}icon_struct_t;

std::vector<gcode_file_t> File_Array;
std::vector<icon_struct_t> Icon_Array;

#define OPEN_DIALOG_BACKGROUND_COLOR LV_COLOR_MAKE(0, 0, 0);
#define OPEN_DIALOG_TEXT_COLOR LV_COLOR_MAKE(255, 255, 255);
lv_obj_t *open_dialog_container = NULL;

static lv_res_t btnm_close_action(lv_obj_t * btnm)
{
  gui_elements_open_dialog_close();
}
static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    //printf("Button %d is released\n", id);
    lv_obj_set_style(Icon_Array[id].button, &lv_style_transp); //Keep the botton from going non-transparent
    gui_elements_open_dialog_close();
    gui_elements_viewer_close_drawing();
    /* The button is released.
     * Make something here */
     printf("Opening file: %s\n", Icon_Array[id].path.c_str());
    gui_elements_viewer_open_drawing(Icon_Array[id].path.c_str());

    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

lv_obj_t *gui_elements_open_dialog(void)
{
  if (open_dialog_container != NULL) return NULL;
  static lv_style_t style;
  lv_style_copy(&style, &lv_style_plain);
  style.body.shadow.width = 3;
  style.body.padding.hor = 0;
  style.body.padding.ver = 0;
  style.body.padding.inner = 0;
  //style.body.border.opa = LV_OPA_100;
  style.body.main_color = OPEN_DIALOG_BACKGROUND_COLOR;
  style.body.grad_color = OPEN_DIALOG_BACKGROUND_COLOR;

  static lv_style_t icon_style;
  lv_style_copy(&icon_style, &lv_style_plain);
  icon_style.body.shadow.width = 0;
  icon_style.body.padding.hor = 0;
  icon_style.body.padding.ver = 0;
  icon_style.body.padding.inner = 0;
  //icon_style.body.border.opa = LV_OPA_100;
  icon_style.body.main_color = OPEN_DIALOG_BACKGROUND_COLOR;
  icon_style.body.grad_color = OPEN_DIALOG_BACKGROUND_COLOR;

  static lv_style_t text_style;
  lv_style_copy(&text_style, &lv_style_plain);
  text_style.text.color = OPEN_DIALOG_TEXT_COLOR;
  text_style.text.font = &lv_font_dejavu_10;

  static lv_style_t button_style;
  lv_style_copy(&button_style, &lv_style_transp);
  memcpy(&lv_style_btn_pr, &lv_style_transp, sizeof(lv_style_t));

  open_dialog_container = lv_win_create(lv_scr_act(), NULL);

  lv_win_set_title(open_dialog_container, "Open Program");
  lv_obj_set_style(open_dialog_container, &style);     /*Set the new style*/
  lv_obj_set_size(open_dialog_container, LV_HOR_RES /2, (LV_VER_RES /2) + 200);
  lv_obj_align(open_dialog_container, NULL, LV_ALIGN_IN_TOP_LEFT, 100, 200);
  lv_win_add_btn(open_dialog_container, SYMBOL_CLOSE, btnm_close_action);

  gui_elements_read_directories();

  LV_IMG_DECLARE(folder_icon);
  LV_IMG_DECLARE(file_icon);

  int x_offset = 30;
  int y_offset = 30;
  icon_struct_t icon;
  for (size_t x = 0; x < File_Array.size(); x++)
  {
    icon.x_offset = x_offset;
    icon.y_offset = y_offset;
    DEBUG_PRINT(("Creating icon for file: %s\n", File_Array[x].name.c_str()));

    icon.container = lv_cont_create(open_dialog_container, NULL);
    icon.image = lv_img_create(icon.container, NULL);
    icon.label = lv_label_create(icon.container, NULL);
    icon.button = lv_btn_create(icon.container, NULL);

    icon.name = File_Array[x].name;
    icon.path = File_Array[x].path;
    Icon_Array.push_back(icon);

    lv_obj_set_style(Icon_Array[Icon_Array.size()-1].container, &icon_style);     /*Set the new style*/
    lv_obj_set_size(Icon_Array[Icon_Array.size()-1].container, 120, 120);
    lv_obj_align(Icon_Array[Icon_Array.size()-1].container, NULL, LV_ALIGN_IN_TOP_LEFT, Icon_Array[Icon_Array.size()-1].x_offset, Icon_Array[Icon_Array.size()-1].y_offset);

    lv_obj_set_free_num(Icon_Array[Icon_Array.size()-1].button, Icon_Array.size()-1);
    lv_obj_set_size(Icon_Array[Icon_Array.size()-1].button, 120, 120);
    lv_btn_set_action(Icon_Array[Icon_Array.size()-1].button, LV_BTN_ACTION_CLICK, btn_click_action);
    lv_obj_set_style(Icon_Array[Icon_Array.size()-1].button, &button_style);
    lv_obj_align(Icon_Array[Icon_Array.size()-1].button, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    if (File_Array[x].name.find(".") != std::string::npos)
    {
      lv_img_set_src(Icon_Array[Icon_Array.size()-1].image, &file_icon);
    }
    else
    {
      lv_img_set_src(Icon_Array[Icon_Array.size()-1].image, &folder_icon);
    }
    lv_obj_align(Icon_Array[Icon_Array.size()-1].image, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_label_set_text(Icon_Array[Icon_Array.size()-1].label, File_Array[x].name.c_str());
    lv_obj_set_style(Icon_Array[Icon_Array.size()-1].label, &text_style);
    lv_obj_align(Icon_Array[Icon_Array.size()-1].label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);


    x_offset += 125;

    if (x_offset > (LV_HOR_RES /2) - 100)
    {
      x_offset = 30;
      y_offset += 130;
    }
  }
  return open_dialog_container;
}
void gui_elements_read_directories(void)
{
  DIR *dpdf;
  struct dirent *epdf;
  gcode_file_t file_struct;
  vector<string> split_path;
  dpdf = opendir(config.post_directory);
  if (dpdf != NULL)
  {
     while (epdf = readdir(dpdf))
     {
        file_struct.path = string(config.post_directory) + "/" + string(epdf->d_name);
        file_struct.name = string(epdf->d_name);
        if (file_struct.name != "." && file_struct.name != "..")
        {
          //printf("Path: %s, Name: %s\n",file_struct.path.c_str(), file_struct.name.c_str());
          File_Array.push_back(file_struct);
        }
     }
  }
  closedir(dpdf);
}
void gui_elements_open_dialog_close()
{
  if (open_dialog_container != NULL)
  {
    lv_obj_del(open_dialog_container);
    open_dialog_container = NULL;
    File_Array.clear();
    Icon_Array.clear();
  }
}
