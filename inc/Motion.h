#ifndef MOTION_
#define MOTION_
#include <application.h>

//X is actually Z here because I smoked the real X!!!! Take note for next setup!

struct ControlPins{
  int M1;
  int M2;
  int M3;
  int DIR;
  int RESET;
  int ENABLE;
  int STEP;
};

#define X_AXIS 0
#define Y_AXIS 1

class Stepper
{
  public:                    // begin public section
    Stepper(int, int);     // constructor
    void SetFeedRate(float);
    void Step(int);
    void Tick();
    void FeedDelay();
  private:
    int total_steps;
    double pulse_delay = INCH_MIN_DELAY; //1 inch/min default
    ControlPins Pins;

};


#endif
