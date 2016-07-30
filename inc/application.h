#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define _USE_MATH_DEFINES
#include <dirent.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cctype>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <array>
#include <iomanip>
#include <locale>
#include <regex>
#include <pthread.h>
#include <sys/ioctl.h>

#include <Config.h>
#include <Render.h>
#include <Serial.h>
#include <Sender.h>

#ifndef APPLICATION_
#define  APPLICATION_

const char APPLICATION_TITLE[] = "PlasmaTable";

#define INPUT 100

struct color_t{
  Uint8 r,g,b,a;
};
struct point_t{
  float x = 0;
  float y = 0;
  float z = 0;
  bool operator!=(const point_t& rhs)
  {
    return x != rhs.x && y != rhs.y && z != rhs.z;
  }
};
#define INDICATOR 0
#define NEEDLE 1
//#define INPUT 2
#define GAUGE 3
#define FILE 4

struct object_t{
  int zindex = 0;
  int type = -1;
  std::string image;
  SDL_Rect position;
  SDL_Rect size;
  double angle;
  bool visable = true;
  SDL_Texture* texture;
  int alpha = 255;
  std::string activity;

  //Parameters
  std::string tagname;
  double minangle;
  double maxangle;
  double minvalue;
  double maxvalue;
  double value;
};
struct script_t{
  std::string name;
  std::vector<std::string> code;
};
extern color_t BackgroundColor;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern SDL_Texture* gTexture;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern std::vector<object_t> ObjectStack;
extern std::vector<script_t> ScriptStack;
extern std::string SerialDevice;
extern int serialfd;
extern bool sim;
extern bool quit;
extern std::string current_activity;
extern std::string current_file;
extern std::string MachineState;
extern float file_open_scroll_offset;
extern std::vector<object_t> file_tiles;
extern float control_signal_pulse;
extern float WPos_X;
extern float WPos_Y;
extern float WPos_Z;

#define KNORMAL  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGREEN  "\x1B[32m"
#define KYELLOW  "\x1B[33m"
#define KBLUE  "\x1B[34m"
#define KMAGENTA  "\x1B[35m"
#define KCYAN  "\x1B[36m"
#define KWHITE  "\x1B[37m"
