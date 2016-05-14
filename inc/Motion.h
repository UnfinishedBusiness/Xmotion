#ifndef MOTION_
#define MOTION_
#include <application.h>

class Stepper
{
  public:                    // begin public section
    Stepper(int, int, int, int, int);     // constructor
    void SetFeedRate(float);
    void Step(int);
    void Tick();
    void FeedDelay();
  private:
    int map[4];
    int total_steps;
    int map_position = 0;
    int step_position = 0;
    double pulse_delay = INCH_MIN_DELAY; //1 inch/min default

    int BitMap[4][4] = {
      { HIGH, LOW, HIGH, LOW },
      { LOW, HIGH, HIGH, LOW },
      { LOW, HIGH, LOW, HIGH },
      { HIGH, LOW, LOW, HIGH },
    };
};


#endif
