#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define _USE_MATH_DEFINES
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

#include <Config.h>
#include <Render.h>
#include <Serial.h>
#include <CNC.h>
#include <Motion.h>

#include <wiringPi.h>

#ifndef APPLICATION_
#define  APPLICATION_

const char APPLICATION_TITLE[] = "PlasmaTable";

#define INCH_MIN_DELAY 23610.2979
#define ONE_STEP_DISTANCE 0.0003935
#define RAPID_FEED 20

struct color_t{
  Uint8 r,g,b,a;
};
struct point_t{
  float x = 0;
  float y = 0;
  float z = 0;
  bool operator==(const point_t& rhs)
  {
    //return x == rhs.x && y == rhs.y && z == rhs.z;
    auto t = [](float a, float b)
    {
      float diff;
      if (a > b)
      {
        diff = a - b;
      }
      else
      {
        diff = b - a;
      }
      //printf("(geoInTolerance) Difference: %.6f, Plus: %.6f, Minus: %.6f\n", diff, fabs(t), -fabs(t));
      if (diff <= fabs(ONE_STEP_DISTANCE) && diff >= -fabs(ONE_STEP_DISTANCE))
      {
        return true;
      }
      else
      {
        return false;
      }
    };
    return t(x, rhs.x) && t(y, rhs.y) && t(z, rhs.z);
  }
  bool operator!=(const point_t& rhs)
  {
    return x != rhs.x && y != rhs.y && z != rhs.z;
  }
};
#define INDICATOR 0
#define NEEDLE 1
//#define INPUT 2
#define GAUGE 3

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
extern long TimeRendered;
extern bool quit;

extern point_t MachineCordinates;
extern point_t OffsetCordinates;
extern point_t OffsetValue;


#define KNORMAL  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGREEN  "\x1B[32m"
#define KYELLOW  "\x1B[33m"
#define KBLUE  "\x1B[34m"
#define KMAGENTA  "\x1B[35m"
#define KCYAN  "\x1B[36m"
#define KWHITE  "\x1B[37m"
