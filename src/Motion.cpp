#include <Motion.h>

using namespace std;

#define CURVE_STEP 0.02
#define INITIAL_CURVE 0.01

float motion_accel = INITIAL_CURVE;
long motion_accel_timer = 0;

ControlPins AxisPins[2] = {
  /*x*/ { 12, 13, 14, 6, 22, 26, 1 },
  /*Y*/ { 0, 2, 3, 5, 21, 11, 16 }
};

Stepper::Stepper(int tsteps, int axis)
{
  total_steps = tsteps; //Not really even used?
  Pins = AxisPins[axis];
  #ifdef NDEBUG
    pinMode(Pins.M1, OUTPUT);
    pinMode(Pins.M2, OUTPUT);
    pinMode(Pins.M3, OUTPUT);
    pinMode(Pins.DIR, OUTPUT);
    pinMode(Pins.RESET, OUTPUT);
    pinMode(Pins.ENABLE, OUTPUT);
    pinMode(Pins.STEP, OUTPUT);

    /* Turn on driver */
    digitalWrite(Pins.ENABLE, HIGH);
  	digitalWrite(Pins.RESET, HIGH);

    /*  1/8 Steps */
  	digitalWrite(Pins.M1, HIGH);
  	digitalWrite(Pins.M2, LOW);
  	digitalWrite(Pins.M3, HIGH);

  #endif

}
void Stepper::SetFeedRate(float feed)
{
  #ifdef NDEBUG
    if ((motion_accel_timer + 200) < millis()) //reset accel curve if its been more than x millis
    {
      motion_accel_timer = millis();
      motion_accel = INITIAL_CURVE;
      //printf("Reseting Curve!\n");
    }
    if (motion_accel < 1) motion_accel += CURVE_STEP;
    pulse_delay = INCH_MIN_DELAY / (feed * motion_accel);
    control_signal_pulse = pulse_delay;
    motion_accel_timer = millis();
  #else
    pulse_delay = INCH_MIN_DELAY / feed;
    control_signal_pulse = pulse_delay;
  #endif
}
void Stepper::Step(int inc)
{
  for (int x = 0; x < abs(inc); x++)
  {
    if (inc > 0)
    {
        #ifdef NDEBUG
          digitalWrite(Pins.DIR, LOW);
          digitalWrite(Pins.STEP, HIGH);
          delayMicroseconds(3);
          digitalWrite(Pins.STEP, LOW);
        #endif
    }
    else
    {
      #ifdef NDEBUG
        digitalWrite(Pins.DIR, HIGH);
        digitalWrite(Pins.STEP, HIGH);
        delayMicroseconds(3);
        digitalWrite(Pins.STEP, LOW);
      #endif
    }
    //FeedDelay();
  }
}
void Stepper::FeedDelay()
{
  #ifdef NDEBUG
  	if (pulse_delay < (INCH_MIN_DELAY / RAPID_FEED))
  	{
      printf("!!!! Feedrate is faster than rapid move!\n");
  		delayMicroseconds(INCH_MIN_DELAY / RAPID_FEED);
  	}
  	else
  	{
      delayMicroseconds(pulse_delay);
  	}
  #else
    if (pulse_delay < (INCH_MIN_DELAY / RAPID_FEED))
    {
      printf("!!!! Feedrate is faster than rapid move!\n");
      usleep(INCH_MIN_DELAY / RAPID_FEED);
    }
    else
    {
      usleep(pulse_delay);
    }
  #endif
}
