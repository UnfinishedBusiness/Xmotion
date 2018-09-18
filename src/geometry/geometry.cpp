#include "geometry.h"

using namespace std;

bool geoInTolerance(geo_point_t a, geo_point_t b, float t)
{
  return (geoInTolerance(a.x, b.x, t) && geoInTolerance(a.y, b.y, t) && geoInTolerance(a.z, b.z, t));
}
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
string geoSupressZeros(float i)
{
  stringstream r;
  r << i;
  return r.str();
}
string geoBoolToString(bool i)
{
  stringstream r;
  r << i;
  return r.str();
}
geo_line_t geoReplaceClosestEndpoint(geo_line_t l, geo_point_t p)
{
  geo_line_t r = l;
  if (geoGetLineLength(geo_line_t{ l.start, p }) < geoGetLineLength(geo_line_t{ l.end, p}))
  {
    r.start = p;
  }
  else
  {
    r.end = p;
  }
  return r;
}
geo_point_t geoGetIntersection(geo_arc_t a, geo_line_t l)
{
  return geoGetLineArcIntersection(a, l);
}
geo_point_t geoGetIntersection(geo_line_t l, geo_arc_t a)
{
  return geoGetLineArcIntersection(a, l);
}
geo_point_t geoGetIntersection(geo_arc_t a1, geo_arc_t a2)
{
  return geoGetArcIntersection(a1, a2);
}
geo_point_t geoGetIntersection(geo_line_t l1, geo_line_t l2)
{
  return geoGetLineIntersection(l1, l2);
}
geo_point_t geoGetLineIntersection(geo_line_t l1, geo_line_t l2)
{
  float a1 = l1.end.y - l1.start.y;
  float b1 = l1.start.x - l1.end.x;
  float c1 = a1 * l1.start.x + b1 * l1.start.y;
  float a2 = l2.end.y - l2.start.y;
  float b2 = l2.start.x - l2.end.x;
  float c2 = a2 * l2.start.x + b2 * l2.start.y;
  float det = a1 * b2 - a2 * b1;
  if (det == 0)
  {
        return geo_point_t{NAN, NAN, NAN}; //Lines are parallel
  }
  float x = (b2 * c1 - b1 * c2) / det;
  float y = (a1 * c2 - a2 * c1) / det;
  return geo_point_t{x, y, 0};
}
geo_point_t geoGetLineArcIntersection(geo_arc_t a, geo_line_t l) //Not really the best way
{
  vector<geo_point_t> av = geoGetPointsOfArc(a);
  vector<geo_point_t> lv = geoGetPointsOfLine(l, 0.0005);
  for (int x = 0; x < lv.size(); x++)
  {
    for (int y = 0; y < av.size(); y++)
    {
      if (geoInTolerance(lv[x], av[y], 0.0001))
      {
        return lv[x];
      }
    }
  }
  return geo_point_t{NAN, NAN, NAN}; //No intersection;*/
}
geo_point_t geoGetArcIntersection(geo_arc_t a1, geo_arc_t a2)
{
  vector<geo_point_t> av1 = geoGetPointsOfArc(a1);
  vector<geo_point_t> av2 = geoGetPointsOfArc(a2);
  for (int x = 0; x < av1.size(); x++)
  {
    for (int y = 0; y < av2.size(); y++)
    {
      if (geoInTolerance(av1[x], av2[y], 0.0001))
      {
        return av1[x];
      }
    }
  }
  return geo_point_t{NAN, NAN, NAN}; //No intersection;
}
bool geoDoLinesIntersect(geo_line_t l1, geo_line_t l2)
{
  // Store the values for fast access and easy
  // equations-to-code conversion
  geo_point_t p1 = l1.start;
  geo_point_t p2 = l1.end;
  geo_point_t p3 = l2.start;
  geo_point_t p4 = l2.end;

  float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
  float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

  float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  // If d is zero, there is no intersection
  if (d == 0) return false;

  // Get the x and y
  float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
  float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
  float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

  // Check if the x and y coordinates are within both lines
  if ( x < min(x1, x2) || x > max(x1, x2) ||
  x < min(x3, x4) || x > max(x3, x4) ) return false;
  if ( y < min(y1, y2) || y > max(y1, y2) ||
  y < min(y3, y4) || y > max(y3, y4) ) return false;

  return true;
}
vector<geo_point_t> geoGetPointsOfLine(geo_line_t l, float scale)
{
  if (scale < 0.0001)
  {
    scale = 0.0001;
  }
  //V printf("(geoGetPointsOfLine) Start(%.6f, %.6f) End(%.6f, %.6f)\n", start.x, start.y, end.x, end.y);
  vector<geo_point_t> p;
  float x = l.end.x - l.start.x;
	float y = l.end.y - l.start.y;
	float length = sqrtf( x*x + y*y );
  //float scale = 0.005;


	float addx = (x / length) * scale;
	float addy = (y / length) * scale;
	x = l.start.x;
	y = l.start.y;
  int count = 0;
	for(float i = 0; i < length; i += scale)
	{
		p.push_back(geo_point_t());
    p[count] = geo_point_t{ x, y };
	  x += addx;
	  y += addy;
    count ++;
	}
  return p;
}
float geoGetLineLength(geo_line_t l)
{
  float x,y,z;
  x = l.end.x - l.start.x;
  y = l.end.y - l.start.y;
  z = l.end.z - l.start.z;
  return sqrtf(x*x + y*y + z*z);
}
geo_point_t geoGetLineMidpoint(geo_line_t l)
{
  geo_point_t m;
  m.x = (l.end.x + l.start.x) / 2;
  m.y = (l.end.y + l.start.y) / 2;
  m.z = (l.end.z + l.start.z) / 2;
  //V printf("(geoGetLineMidpoint) Midpoint of (%.6f, %.6f) --- (%.6f, %.6f) is (%.6f, %.6f)\n", l.start.x, l.start.y, l.end.x, l.end.y, m.x, m.y);
  return m;
}
geo_point_t geoRotatePointAroundPoint(geo_point_t p, geo_point_t o, float angle, bool dir) //angle is in degrees!
{
  //Clockwise is negative and ccw is positive, backword to logic!
  float a;
  if (dir == ARC_CW)
  {
    a = -angle;
  }
  else
  {
    a = POSITIVE(angle);
  }
  return geoRotatePointAroundPoint(p, o, a);
}
geo_point_t geoRotatePointAroundPoint(geo_point_t p, geo_point_t o, float angle) //angle is in degrees!
{
  float rad = angle * (3.14159265359 / 180.0);
  return geo_point_t{ geoRound(cosf(rad) * (p.x - o.x) - sinf(rad) * (p.y - o.y) + o.x),
                  geoRound(sinf(rad) * (p.x - o.x) + cosf(rad) * (p.y - o.y) + o.y), 0 };
}
geo_point_t geoTranslatePointByVector(geo_point_t o, geo_point_t v)
{
  o.x -= v.x;
  o.y -= v.y;
  return o;
}
geo_point_t geoTranslateCalculateVector(geo_point_t o, geo_point_t t)
{
  return geo_point_t{ (o.x - t.x), (o.y - t.y) };
}
float geoRadiansToDegrees(float r)
{
  return (r * 57.2957795);
}
float geoDegreesToRadians(float r)
{
  return 2 * M_PI * (r / 360);
}

float geoGetLineAngle(geo_line_t l)
{
  float angle = atan2f(l.start.y - l.end.y, l.start.x - l.end.x);
  return angle;
}
geo_line_t geoRotateLine(geo_line_t l, geo_point_t o, float a)
{
  return geo_line_t{ geoRotatePointAroundPoint(l.start, o, a), geoRotatePointAroundPoint(l.end, o, a) };
}
geo_line_t geoExtendLine(geo_line_t l, float d)
{
  float angle = geoGetLineAngle(l);
  geo_point_t new_startpoint = geo_point_t{ l.start.x + (fabs(d) * cosf(angle)), l.start.y + (fabs(d) * sinf(angle)) };
  geo_point_t new_endpoint = geo_point_t{ l.end.x + (fabs(d) * cosf(angle)), l.end.y + (fabs(d) * sinf(angle)) };
  float new_startpoint_distance = geoGetLineLength(geo_line_t{ l.start, new_startpoint });
  float new_endpoint_distance = geoGetLineLength(geo_line_t{ l.end, new_endpoint });
  if (new_startpoint_distance > new_endpoint_distance)
  {
    //V printf("(geoExtendLine) Entending from start point\n");
    return geo_line_t{ new_startpoint, l.end };
  }
  else
  {
    //V printf("(geoExtendLine) Entending from end point\n");
    return geo_line_t{ l.start, new_endpoint };
  }
}
geo_line_t geoExtendLineEndpoint(geo_line_t l, float d)
{
  float angle = geoGetLineAngle(l);
  geo_point_t new_endpoint = geo_point_t{ l.end.x + (fabs(d) * cosf(angle)), l.end.y + (fabs(d) * sinf(angle)) };
  return geo_line_t{ l.start, new_endpoint };
}
geo_line_t geoExtendLineStartpoint(geo_line_t l, float d)
{
  float angle = geoGetLineAngle(l);
  geo_point_t new_endpoint = geo_point_t{ l.start.x + (fabs(d) * cosf(angle)), l.start.y + (fabs(d) * sinf(angle)) };
  return geo_line_t{ new_endpoint, l.start };
}
geo_line_t geoExtendLineAngle(geo_point_t s, float angle, float d) //Angle is in radians
{
  geo_point_t new_endpoint = geo_point_t{ s.x + (fabs(d) * cosf(angle)), s.y + (fabs(d) * sinf(angle)) };
  return geo_line_t{ s, new_endpoint };
}
geo_line_t geoFlipLine(geo_line_t l)
{
  return geo_line_t{l.end, l.start};
}
geo_arc_t geoFlipArc(geo_arc_t a)
{
  geo_arc_t r = a;
  r.direction = !a.direction;
  r.start = a.end;
  r.end = a.start;
  r.center = a.center;
  //V debugDumpArcStructure(r);
  return r;
}
float geoGetPerpendicularDistance(geo_line_t l , geo_point_t p)
{
  /*geo_line_t perp = geoGetPerpendicularLine(l, p, 1); //Just give it any length, we have to find the intersection point
  float l_length = geoGetLineLength(l);
  float angle = geoGetLineAngle(l);
  geo_line_t parallel_line = geoExtendLineAngle(p, angle, l_length / 2);
  geo_point_t intersection = geoGetIntersection(parallel_line, perp);
  return geoGetLineLength(geo_line_t{intersection, geoGetLineMidpoint(l)});*/
  // Return minimum distance between line segment vw and point p

  //double FindDistanceToSegment(double x1, double y1, double x2, double y2, double pointX, double pointY)
  float x1 = l.start.x;
  float y1 = l.start.y;
  float x2 = l.end.x;
  float y2 = l.end.y;
  float pointX = p.x;
  float pointY = p.y;
  float diffX = x2 - x1;
  float diffY = y2 - y1;
  if ((diffX == 0) && (diffY == 0))
  {
      diffX = pointX - x1;
      diffY = pointY - y1;
      return sqrtf(diffX * diffX + diffY * diffY);
  }
  float t = ((pointX - x1) * diffX + (pointY - y1) * diffY) / (diffX * diffX + diffY * diffY);
  if (t < 0)
  {
      //point is nearest to the first point i.e x1 and y1
      diffX = pointX - x1;
      diffY = pointY - y1;
  }
  else if (t > 1)
  {
      //point is nearest to the end point i.e x2 and y2
      diffX = pointX - x2;
      diffY = pointY - y2;
  }
  else
  {
      //if perpendicular line intersect the line segment.
      diffX = pointX - (x1 + t * diffX);
      diffY = pointY - (y1 + t * diffY);
  }
  //returning shortest distance
  return sqrtf(diffX * diffX + diffY * diffY);

}
geo_line_t geoGetPerpendicularLine(geo_line_t l, geo_point_t direction, float d)
{
  int angle;
  geo_point_t midpoint = geoGetLineMidpoint(l);
  //Get lines perpendicular on both sides and see which enpoint is closest to direction point
  geo_line_t r;
  geo_line_t r270 = geo_line_t{ midpoint, geoRotatePointAroundPoint(l.start, midpoint, 270) };
  geo_line_t r90 = geo_line_t{ midpoint, geoRotatePointAroundPoint(l.start, midpoint, 90) };
  float r270_d = geoGetLineLength(geo_line_t{ r270.end, direction });
  float r90_d = geoGetLineLength(geo_line_t{ r90.end, direction });
  //V printf("(geoGetPerpendicularLine) R270 Distance: %.6f, R90 Distance: %.6f\n", r270_d, r90_d);
  if ( r90_d < r270_d )
  {
    //V printf("(geoGetPerpendicularLine) R270 is closer!\n");
    r = r270;
  }
  else
  {
    //V printf("(geoGetPerpendicularLine) R90 is closer!\n");
    r = r90;
  }
  float a = geoGetLineAngle(r);
  geo_point_t new_endpoint = geo_point_t{ midpoint.x + (fabs(d) * cosf(a)), midpoint.y + (fabs(d) * sinf(a)) };
  return geo_line_t{ midpoint, new_endpoint };
}
geo_line_t geoGetParallelLine(geo_line_t l, geo_point_t direction, float d)
{
  geo_line_t perp = geoGetPerpendicularLine(l, direction, d);
  geo_point_t midpoint = geoGetLineMidpoint(l);
  geo_line_t r1 = geoExtendLineAngle(perp.end, geoGetLineAngle(l), geoGetLineLength(geo_line_t{midpoint, l.end})); //Get half of line
  geo_line_t r2 = geoExtendLineAngle(r1.end, geoGetLineAngle(l) + 3.14159265, geoGetLineLength(geo_line_t{l.end, l.start})); //Get rest of line
  return r2;
}
geo_point_t geoGetArcEndpoint(geo_arc_t a, float length)
{
    float r = sqrtf(powf(a.start.x - a.center.x, 2) + powf(a.start.y - a.center.y, 2));
    float angle = atan2(a.start.y - a.center.y, a.start.x - a.center.x);
    if (a.direction == ARC_CW) {
        angle = angle - length / r;
    }
    else {
        angle = angle + length / r;
    }
    return geo_point_t{a.center.x + r * cos(angle), a.center.y + r * sin(angle) };
}
float geoGetArcStartAngle(geo_arc_t a)
{
  return atan2(a.start.y - a.center.y, a.start.x - a.center.x);
}
float geoGetArcEndAngle(geo_arc_t a)
{
  if (a.direction == ARC_CCW && geoGetArcStartAngle(a) > 0 && (atan2(a.end.y - a.center.y, a.end.x - a.center.x) + 6.28318531) < 6.28318531)
  {
    return atan2(a.end.y - a.center.y, a.end.x - a.center.x) + 6.28318531;
  }
  else
  {
    return atan2(a.end.y - a.center.y, a.end.x - a.center.x);
  }

}
float geoGetArcStartAngleAbs(geo_arc_t a)
{
  //Draw line from arc center to start point and calculate angle of line
  float angle = geoRadiansToDegrees(geoGetLineAngle(geo_line_t{ a.center, a.start}));
  //cout << KRED << "(geoGetArcStartAngleAbs) Angle = " << angle << KNORMAL << endl;
  return angle;
}
float geoGetArcEndAngleAbs(geo_arc_t a)
{
  float angle = geoRadiansToDegrees(geoGetLineAngle(geo_line_t{ a.center, a.end}));
  //cout << KRED << "(geoGetArcStartAngleAbs) Angle = " << angle << KNORMAL << endl;
  return angle;
}
geo_point_t geoGetArcPoint(geo_arc_t a, float angle) //Angle is in degrees
{
  geo_point_t e = geo_point_t{ a.radius + a.center.x, a.center.y, 0 };
  return geoRotatePointAroundPoint(e, a.center, angle);
}
std::vector<geo_point_t> geoGetPointsOfArc(geo_arc_t a)
{
  std::vector<geo_point_t> r;
  int rx = 0;
  float includedAngle = geoGetIncludedAngle(a);
  geo_line_t l = geo_line_t{ geo_point_t{a.center.x, a.center.y,} , geo_point_t{a.start.x, a.start.y} };
  if (a.start != a.end)
  {
    r.push_back(geo_point_t());
    r[rx] = a.start;
    rx++;
  }
  int steps;
  if (a.start == a.end)
  {
    //We're a circle
    steps = 361; //Go the full 360 to close the gap
  }
  else
  {
    steps = geoGetIncludedAngle(a);
  }
  float inc_angle = 1; //Degrees
  for (int x=0; x < steps; x+=inc_angle)
  {
    if (a.direction == ARC_CW)
    {
      l = geoRotateLine(l, a.center, -inc_angle);
    }
    else
    {
      l = geoRotateLine(l, a.center, inc_angle);
    }
    r.push_back(geo_point_t());
    r[rx] = l.end;
    rx++;
  }
  if (a.start != a.end)
  {
    r.push_back(geo_point_t());
    r[rx] = a.end;
    rx++;
  }
  return r;
}
float geoGetArcLength(geo_arc_t a) //Not varified
{
  float angle = geoGetIncludedAngle(a);
  return a.radius * angle;
}
geo_point_t geoGetArcMidpoint(geo_arc_t a)
{
  float c=atan2(a.start.y + a.end.y, a.start.x + a.end.x);
  return geo_point_t{a.radius * cosf(c) + a.center.x, a.radius * sinf(c) + a.center.y};
}
float geoGetIncludedAngle(geo_arc_t a)
{
  float angle;
  /*if (a.direction == ARC_CCW)
  {
    angle = geoGetArcEndAngle(a) - geoGetArcStartAngle(a);
  }
  else
  {
    angle = geoGetArcStartAngle(a) - geoGetArcEndAngle(a) + geoDegreesToRadians(360);
  }*/
  angle = geoGetArcEndAngle(a) - geoGetArcStartAngle(a);
  if (angle == 0)
  {
    return 360;
  }
  else
  {
    return geoRadiansToDegrees(fabs(angle));
  }
}
bool geoGetArcDirection(float s, float e)
{
  float direction;
  if (s < e)
  {
    direction = ARC_CCW;
  }
  else
  {
    direction = ARC_CW;
  }
  /*if ((360 + (int)s - (int)e) % 360 < 180)
  {
    //cout << "True!\n";
    direction = !direction;
  }*/
  return direction;
}
float geoRound(float x) //Round to 6 places
{
   return round(1000000 * x) / 1000000;
}
geo_arc_t geoGetCircle(geo_point_t p, float r)
{
  geo_arc_t c;
  c.radius = r;
  c.center = p;
  //V debugDumpPointStructure(p);
  c.start = p;
  c.start.x += c.radius;
  c.end = c.start;
  c.direction = ARC_CW;
  return c;
}
geo_point_t geoScalePoint(geo_point_t p, float s)
{
  p.x = p.x * s;
  p.y = p.y * s;
  p.z = p.z * s;
  return p;
}
