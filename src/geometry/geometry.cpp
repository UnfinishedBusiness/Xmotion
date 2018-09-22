#include "geometry.h"
#include <math.h>

#define _USE_MATH_DEFINES

using namespace std;

bool geoInTolerance(float a, float b, float t)
{
  float diff;
  if (a > b)
  {
    diff = a - b;
  }
  else
  {
    diff = b - a;
  }
  //printf("(geoInTolerance) Difference: %.6f, Plus: %.6f, Minus: %.6f\n", diff, fabs(t), -fabs(t));
  if (diff <= fabs(t) && diff >= -fabs(t))
  {
    return true;
  }
  else
  {
    return false;
  }
}
float geoGetLineLength(geo_line_t l)
{
  float x,y;
  x = l.end.x - l.start.x;
  y = l.end.y - l.start.y;
  //z = l.end.z - l.start.z;
  return sqrtf(x*x + y*y);
}
geo_point_t geoRotatePoint(geo_point_t pivot, geo_point_t rotated_point, float angle)
{
  float s = sin(angle*M_PI/180);
  float c = cos(angle*M_PI/180);

  // translate point back to origin:
  rotated_point.x -= pivot.x;
  rotated_point.y -= pivot.y;

  // rotate point
  float xnew = rotated_point.x * c - rotated_point.y * s;
  float ynew = rotated_point.x * s + rotated_point.y * c;

  // translate point back:
  rotated_point.x = xnew + pivot.x;
  rotated_point.y = ynew + pivot.y;
  return rotated_point;
}
float geoGetLineAngle(geo_line_t l)
{
  float angle = atan2(l.start.y - l.end.y, l.start.x - l.end.x);
  float ret =  (angle * (180.0/M_PI)) + 180;
  if (ret >= 360) ret -= 360;
  return ret;
}
geo_line_t geoGetPolarLine(geo_point_t start_point, float angle, float length)
{
  geo_line_t line;
  line.start.x = start_point.x;
  line.start.y = start_point.y;

  line.end.x = start_point.x + length;
  line.end.y = start_point.y;

  geo_point_t new_end = geoRotatePoint(start_point, line.end, angle);

  line.end.x = new_end.x;
  line.end.y = new_end.y;

  return line;
}
std::vector<geo_point_t> geoGetPointsOfArc(geo_arc_t a)
{
  std::vector<geo_point_t> r;
  geo_line_t start_line = {a.center, a.start};
  geo_line_t end_line = {a.center, a.end};
  float start_angle = geoGetLineAngle(start_line);
  float end_angle = geoGetLineAngle(end_line);
  float angle_inc = 1;

  //printf("start_angle: %0.4f\n", start_angle);
  //printf("end_angle: %0.4f\n", end_angle);
  //Clockwise arc we negative increment angle and Counter-Clockwise angle we posative increment angle!
  //return r;
  r.push_back(start_line.end);
  if (start_angle == end_angle) //We are a circle
  {
    for (float x = 0; x < 360; x += angle_inc)
    {
      start_angle += angle_inc;
      geo_line_t polar = geoGetPolarLine(a.center, start_angle, a.radius);
      r.push_back(polar.end);
    }
  }
  else
  {
    if (a.direction == ARC_CW)
    {
      while(true)
      {
        start_angle -= angle_inc;
        if (start_angle <= 0) start_angle = 360;
        if (geoInTolerance(start_angle, end_angle, angle_inc * 2)) break; //End of arc, break loop!
        geo_line_t polar = geoGetPolarLine(a.center, start_angle, a.radius);
        r.push_back(polar.end);
      }
    }
    else
    {
      while(true)
      {
        start_angle += angle_inc;
        if (start_angle >= 360) start_angle = 0;
        if (geoInTolerance(start_angle, end_angle, angle_inc * 2)) break; //End of arc, break loop!
        geo_line_t polar = geoGetPolarLine(a.center, start_angle, a.radius);
        r.push_back(polar.end);
      }
    }
  }
  r.push_back(end_line.end);


  return r;
}
