#ifndef CNC_
#define CNC_
#include <application.h>

struct gline_meta_t{
  point_t start_pos;
};

struct garc_meta_t{
  point_t start_pos;
};

struct gcode_t{
  float G;
  float X;
  float Y;
  float Z;
  float F;
  float R;
  
  bool MoveDone;

  bool FirstInstruction;
  gline_meta_t line_meta;
  garc_meta_t arc_meta;
};


void CNC_JogXPlus();
void CNC_JogXMinus();
void CNC_JogYPlus();
void CNC_JogYMinus();
void CNC_Hold();
void CNC_Stop();
void CNC_Start();
void CNC_SetOrigin();

void CNC_Tick();
void CNC_Init();
#endif
