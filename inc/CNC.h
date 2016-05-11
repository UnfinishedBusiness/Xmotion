#ifndef CNC_
#define CNC_
#include <application.h>

struct gcode_t{
  float G;
  float X;
  float Y;
  float Z;
  float F;

  bool MoveDone;
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
