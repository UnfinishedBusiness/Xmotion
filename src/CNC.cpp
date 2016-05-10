#include <CNC.h>

using namespace std;

point_t PosPointer;
gcode_t GcodePointer;
bool Hold = false;
bool Stop = false;
Stepper *Xaxis;

void CNC_JogXPlus()
{
  Xaxis->Step(+1);
}
void CNC_JogXMinus()
{
  Xaxis->Step(-1);
}
void CNC_JogYPlus()
{

}
void CNC_JogYMinus()
{

}
void CNC_Hold()
{
  Hold = true;
}
void CNC_Stop()
{
  Stop = true;
}
void CNC_Start()
{
  Hold = false;
}

void CNC_Tick()
{
  if (GcodePointer.MoveDone == false)
  {
    if (Hold == false)
    {
      //Drive axis to position and set GcodePointer.MoveDone = true;
    }
  }
  else
  {
    //Move is done, read nc file for next instruction, then set GcodePointer.MoveDone = false;
  }
}
void CNC_Init()
{
  wiringPiSetup();
  Xaxis = new Stepper(200, 29, 28, 27, 26);
}
