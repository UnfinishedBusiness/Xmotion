#ifndef GEOMETRY_
#define GEOMETRY_

#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cctype>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <array>
#include <iomanip>
#include <locale>

//Angle 0 degrees is on x horizon
#define GEO_REAL_WORLD_CLOSE 0.00005 //Half a tenth
#define GEO_ARC 0x00
#define GEO_LINE 0x01
#define GEO_NOTE 0x02
#define GEO_DIMENSION 0x03
#define DIMENSION_POINT 0x00
#define ARC_CW true
#define ARC_CCW false
#define CONTOUR_RIGHT true
#define CONTOUR_LEFT false


#define ZERO 1e-10
#define SMALL 0.1
#define DELTA 1e-10
#define isZero(A) ( (A < ZERO) && (A > -ZERO) )
#define isSmall(A) ( (A < SMALL) && (A > -SMALL) )
#define isSame(A, B) ( ((A-B) < ZERO) && ((A-B) > -ZERO) )
#define isSimilar(A, B) ( ((A-B) < SMALL) && ((A-B) > -SMALL) )
#define isBetween(A, B, C) ( ((A-B) > -ZERO) && ((A-C) < ZERO) )
#define POSITIVE(n) ((n) < 0 ? 0 - (n) : (n))

struct geo_point_t{
  float x,y,z;
  bool operator==(const geo_point_t& rhs)
  {
    //return x == rhs.x && y == rhs.y && z == rhs.z;
    auto t = [](float a, float b)
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
      if (diff <= fabs(GEO_REAL_WORLD_CLOSE) && diff >= -fabs(GEO_REAL_WORLD_CLOSE))
      {
        return true;
      }
      else
      {
        return false;
      }
    };
    return t(x, rhs.x) && t(y, rhs.y) && t(z, rhs.z);
  }
  bool operator!=(const geo_point_t& rhs)
  {
    return x != rhs.x && y != rhs.y && z != rhs.z;
  }
  int parrentIndex;
  float tmp;
};
struct geo_line_t{
  geo_point_t start;
  geo_point_t end;

  //Operation data
  int parentIndex;
};
struct geo_arc_t{
  geo_point_t start;
  geo_point_t end;
  geo_point_t center;
  float radius;
  bool direction;

  //Operation data
  int parentIndex;
};
geo_line_t geoGetPolarLine(geo_point_t, float , float);
std::vector<geo_point_t> geoGetPointsOfArc(geo_arc_t);
geo_point_t geoRotatePoint(geo_point_t, geo_point_t, float);
geo_line_t geoGetPolarLine(geo_point_t, float, float);
float geoGetLineLength(geo_line_t);
#endif
