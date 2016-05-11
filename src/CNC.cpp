#include <CNC.h>

using namespace std;

point_t MachineCordinates;
point_t OffsetCordinates;
point_t OffsetValue;

gcode_t GcodePointer;
bool Hold = false;
bool Stop = false;
Stepper *Xaxis;
Stepper *Yaxis;

void CNC_JogXPlus()
{
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(+1);
  MachineCordinates.x += ONE_STEP_DISTANCE;
}
void CNC_JogXMinus()
{
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(-1);
  MachineCordinates.x -= ONE_STEP_DISTANCE;
}
void CNC_JogYPlus()
{
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(+1);
  MachineCordinates.y += ONE_STEP_DISTANCE;
}
void CNC_JogYMinus()
{
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(-1);
  MachineCordinates.y -= ONE_STEP_DISTANCE;
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
void CNC_SetOrigin()
{
  //printf("Setting Origin!\n");
  OffsetValue.x = MachineCordinates.x;
  OffsetValue.y = MachineCordinates.y;
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
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
}
void CNC_Init()
{
  wiringPiSetup();
  Xaxis = new Stepper(200, 29, 28, 27, 26);
  Yaxis = new Stepper(200, 29, 28, 27, 26);

  //Home maching to establish Machine Cordinates

  MachineCordinates.x = 0;
  MachineCordinates.y = 0;

  OffsetCordinates.x = 0;
  OffsetCordinates.y = 0;

  OffsetValue.x = 0;
  OffsetValue.y = 0;


}
