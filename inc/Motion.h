#ifndef MOTION_
#define MOTION_
#include <application.h>

class Stepper
{
  public:                    // begin public section
    Stepper(int, int, int, int, int);     // constructor
    void SetRPM(float);
    void Step(int);
    void Tick();
  private:
    int map[4];
    int total_steps;
    int map_position = 0;
    int step_position = 0;
    int current_rpm = 0;
};


#endif
