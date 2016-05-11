#include <Motion.h>

using namespace std;

Stepper::Stepper(int tsteps, int a, int b, int c, int d)
{
  total_steps = tsteps;
  map[0] = a;
  map[1] = b;
  map[2] = c;
  map[3] = d;
  for (int x = 0; x < 4; x++)
  {
    pinMode(map[x], OUTPUT);
  }
}
void Stepper::SetFeedRate(float feed)
{
  pulse_delay = INCH_MIN_DELAY / feed;
}
void Stepper::Step(int inc)
{
  int last_map_position = map_position;
  for (int x = 0; x < abs(inc); x++)
  {
    if (inc > 0)
    {
        map_position++;

    }
    else
    {
        map_position--;
    }
    if (map_position == 4) map_position = 0;
    if (map_position == -1) map_position = 3;

    //printf("Map Position: %d\n", map_position);
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(map[i], BitMap[map_position][i]);
    }
    //delay(1); //Works!
    delayMicroseconds(pulse_delay);
    //delayMicroseconds(500); //Doesnt work!

  }
}
