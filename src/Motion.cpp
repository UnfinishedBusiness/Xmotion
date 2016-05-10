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
void Stepper::SetRPM(float rpm)
{
  current_rpm = rpm;
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

    printf("Map Position: %d\n", map_position);
    digitalWrite(map[last_map_position], LOW);
    digitalWrite(map[map_position], HIGH);
    delay(10);

  }
}
