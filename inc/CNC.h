#ifndef CNC_
#define CNC_
#include <application.h>

struct gline_meta_t{
  point_t start_pos;
};

struct garc_meta_t{
  point_t start_pos;
  point_t center_pos;
  point_t last_pos;

  bool full_circle;


  //int number_of_steps;
  int step_count;

  float end_angle;
  float current_angle;
};

struct gcode_t{
  float G;
  float X;
  float Y;
  float Z;
  float F;
  float R;

  float I;
  float J;
  float K;

  bool Modal;


  bool MoveDone;
  bool FirstInstruction;
  gline_meta_t line_meta;
  garc_meta_t arc_meta;
};

extern gcode_t GcodePointer;

void CNC_JogXPlus();
void CNC_JogXMinus();
void CNC_JogYPlus();
void CNC_JogYMinus();
void CNC_JogZPlus();
void CNC_JogZMinus();
void CNC_Hold();
void CNC_Stop();
void CNC_Start();

void CNC_SetOrigin();
void CNC_SetZOrigin();

void CNC_Tick();
void CNC_Init();
#endif
