#include <CNC.h>

using namespace std;

point_t MachineCordinates;
point_t OffsetCordinates;
point_t OffsetValue;

gcode_t GcodePointer;


bool Hold = true;
bool Stop = true;
bool Cutting = false;
Stepper *Xaxis;
Stepper *Yaxis;
#define CUTTING_HEAD 23
string current_file;

ifstream nc_file;
long nc_line = 0;
string nc_buffer;

void CNC_AnyStep()
{
  #ifdef DEBUG
    if (Cutting == true)
    {
      fprintf(stderr, "%.6f %.6f\n", OffsetCordinates.x, OffsetCordinates.y);
      //Debug_PlotPoint(OffsetCordinates);
    }
    //usleep(200);
  #endif
}
void CNC_XPlus()
{
  MachineCordinates.x += ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
  CNC_AnyStep();
}
void CNC_XMinus()
{
  MachineCordinates.x -= ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
  CNC_AnyStep();
}
void CNC_YPlus()
{
  MachineCordinates.y += ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
  CNC_AnyStep();
}
void CNC_YMinus()
{
  MachineCordinates.y -= ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
  CNC_AnyStep();
}

void CNC_JogXPlus()
{
  if (Stop == false) return;
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(+1);
  Xaxis->FeedDelay();
  CNC_XPlus();
  //Xaxis->FeedDelay();
}
void CNC_JogXMinus()
{
  if (Stop == false) return;
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(-1);
  Xaxis->FeedDelay();
  CNC_XMinus();
  //Xaxis->FeedDelay();
}
void CNC_JogYPlus()
{
  if (Stop == false) return;
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(+1);
  Yaxis->FeedDelay();
  CNC_YPlus();
  //Yaxis->FeedDelay();
}
void CNC_JogYMinus()
{
  if (Stop == false) return;
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(-1);
  Yaxis->FeedDelay();
  CNC_YMinus();
  //Yaxis->FeedDelay();
}
void CNC_Hold()
{
  Hold = true;
  printf("Hold!\n");
}
void CNC_Stop()
{
  Stop = true;
  printf("Stop!\n");
  nc_file.close();
}
void CNC_Start()
{
  if (!nc_file.is_open())
  {
    Stop = false;
    if (current_file != "")
    {
      //current_file = current_file + "/media/usb/";
      nc_file.open(current_file);
    }
  }
  Hold = false;
  printf("Start!\n");
}
void CNC_SetOrigin()
{
  printf("Setting Origin!\n");
  if (Stop == true) //We can't set origin while program is running!
  {
    OffsetValue.x = MachineCordinates.x;
    OffsetValue.y = MachineCordinates.y;

    OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
    OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
  }
}
bool InTolerance(float a, float b, float t)
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
float GetLineLength(point_t p1, point_t p2)
{
  float x,y;
  x = p2.x - p1.x;
  y = p2.y - p1.y;
  return sqrtf(x*x + y*y);
}

float feed = 0;

float fxy, dx, dy = 0;
float x2, y2 = 0;
float xo, yo = 0;
//true = 1
float angle;
point_t next_point;

bool d, f, a, b = 0;
void CNC_BlockingLine(point_t from, point_t to)
{
  fxy = dx = dy = y2 = x2 = xo = yo = 0;
  dy = to.y - from.y;
  if (dy < 0)
  {
    yo = -1;
  }
  else
  {
    yo = 1;
  }
  dy = fabs(dy);

  dx = to.x - from.x;
  if (dx < 0)
  {
    xo = -1;
  }
  else
  {
    xo = 1;
  }
  dx = fabs(dx);
  fxy = dx - dy;
  while(true) //Tick until we reach end point!
  {
    //printf("FXY: %0.4f\n", fxy);
    if (fxy > 0)
    {
      if (xo > 0)
      {
        Xaxis->SetFeedRate(feed);
        Xaxis->Step(+1);
        CNC_XPlus();
      }
      else
      {
        Xaxis->SetFeedRate(feed);
        Xaxis->Step(-1);
        CNC_XMinus();
      }
      x2++;
      fxy = fxy - dy;
    }
    else
    {
      if (yo > 0)
      {
        Yaxis->SetFeedRate(feed);
        Yaxis->Step(+1);
        CNC_YPlus();
      }
      else
      {
        Yaxis->SetFeedRate(feed);
        Yaxis->Step(-1);
        CNC_YMinus();
      }
      y2++;
      fxy = fxy + dx;
    }
    Yaxis->FeedDelay(); //Both axis should be the same feed rate!
    /*if (InTolerance(OffsetCordinates.x, to.x, (ONE_STEP_DISTANCE + 0.0001)) && InTolerance(OffsetCordinates.y, to.y, (ONE_STEP_DISTANCE + 0.0001)))
    {
      MachineCordinates.x += (to.x - OffsetCordinates.x);
      MachineCordinates.y += (to.y - OffsetCordinates.y);
      OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
      OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
      return;
    }*/
    if (GetLineLength(OffsetCordinates, to) <= (ONE_STEP_DISTANCE + 0.0002))
    {
      MachineCordinates.x += (to.x - OffsetCordinates.x);
      MachineCordinates.y += (to.y - OffsetCordinates.y);
      OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
      OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
      return;
    }
  }
}
bool StartsWith(string text, string token)
{
      if(text.length() < token.length())
      {
        return false;
      }
      return (text.compare(0, token.length(), token) == 0);
}
void MoveDone()
{
  //OffsetCordinates.x = GcodePointer.X;
  //OffsetCordinates.y = GcodePointer.Y;

  //Comp for real number versus machine capable number
  MachineCordinates.x += (GcodePointer.X - OffsetCordinates.x);
  MachineCordinates.y += (GcodePointer.Y - OffsetCordinates.y);
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;


  GcodePointer.MoveDone = true;
  //Hold = true;
}
float LastLength = 0;
void CNC_Tick()
{
  if (nc_file.is_open())
  {
    if (GcodePointer.MoveDone == false)
    {
      //printf("Move Done = false\n");
      if (Hold == false)
      {
        if (GcodePointer.G == 0 || GcodePointer.G == 1)
        {
          if (GcodePointer.G == 0)
          {
            feed = RAPID_FEED;
          }
          else
          {
            feed = GcodePointer.F;
          }
          if (GcodePointer.FirstInstruction == true)
          {
            //Calculate meta_data
            fxy = dx = dy = y2 = x2 = xo = yo = 0;

            GcodePointer.line_meta.start_pos.x = OffsetCordinates.x;
            GcodePointer.line_meta.start_pos.y = OffsetCordinates.y;
            GcodePointer.FirstInstruction = false;

            dy = GcodePointer.Y - GcodePointer.line_meta.start_pos.y;
            if (dy < 0)
            {
              yo = -1;
            }
            else
            {
              yo = 1;
            }
            dy = fabs(dy);

            dx = GcodePointer.X - GcodePointer.line_meta.start_pos.x;
            if (dx < 0)
            {
              xo = -1;
            }
            else
            {
              xo = 1;
            }
            dx = fabs(dx);

            fxy = dx - dy;

            //printf("\tDirection -> dx = %0.4f, dy = %0.4f, yo = %0.4f, xo = %0.4f, initial_fxy = %0.4f\n", dx, dy, yo, xo, fxy);
            printf("\tMove to X%0.4f, Y%0.4f\n", GcodePointer.X, GcodePointer.Y);

          }
          else
          {
            //printf("FXY: %0.4f\n", fxy);
            if (fxy > 0)
            {
              if (xo > 0)
              {
                Xaxis->SetFeedRate(feed);
                Xaxis->Step(+1);
                CNC_XPlus();
              }
              else
              {
                Xaxis->SetFeedRate(feed);
                Xaxis->Step(-1);
                CNC_XMinus();
              }
              x2++;
              fxy = fxy - dy;
            }
            else
            {
              if (yo > 0)
              {
                Yaxis->SetFeedRate(feed);
                Yaxis->Step(+1);
                CNC_YPlus();
              }
              else
              {
                Yaxis->SetFeedRate(feed);
                Yaxis->Step(-1);
                CNC_YMinus();
              }
              y2++;
              fxy = fxy + dx;
            }
            Yaxis->FeedDelay(); //Both axis should be the same feed rate!
          }
          point_t end;
          end.x = GcodePointer.X;
          end.y = GcodePointer.Y;
          if ( GetLineLength(OffsetCordinates, end) <= (ONE_STEP_DISTANCE + 0.0002))
          {
            printf("\t\tReached line endpoint!\n");
            MoveDone();
          }
          else
          {
            if (InTolerance(OffsetCordinates.x, GcodePointer.X, (ONE_STEP_DISTANCE + 0.0002)) && InTolerance(OffsetCordinates.y, GcodePointer.Y, (ONE_STEP_DISTANCE + 0.0002)))
            {
              printf("\t\tReached line endpoint incorectly!\n");
              MoveDone();
            }
          }
        }
        if (GcodePointer.G == 2 || GcodePointer.G == 3)
        {
          if (GcodePointer.FirstInstruction == true)
          {
            GcodePointer.arc_meta.start_pos = OffsetCordinates;
            GcodePointer.arc_meta.last_pos = OffsetCordinates;
            //GcodePointer.arc_meta.center_pos.x = OffsetCordinates.x + GcodePointer.I;
            //GcodePointer.arc_meta.center_pos.y = OffsetCordinates.y + GcodePointer.J;
            GcodePointer.FirstInstruction = false;
            GcodePointer.arc_meta.end_angle = atan2(GcodePointer.arc_meta.center_pos.y - GcodePointer.Y, GcodePointer.arc_meta.center_pos.x - GcodePointer.X);

            //GcodePointer.arc_meta.number_of_steps = 6.28319 / ARC_RESOLUTION;
            //GcodePointer.arc_meta.step_position = 0;

            if (GcodePointer.G == 2)
            {
              angle = -ARC_RESOLUTION;
              printf("\tClockwise Arc until we reach X%0.4f, Y%0.4f at F%0.4f\n", GcodePointer.X, GcodePointer.Y, GcodePointer.F);
            }
            if (GcodePointer.G == 3)
            {
              angle = ARC_RESOLUTION;
              printf("\tCounter-Clockwise Arc until we reach X%0.4f, Y%0.4f at F%0.4f\n", GcodePointer.X, GcodePointer.Y, GcodePointer.F);
            }
            printf("\t\tWith Center X: %0.4f, Y: %0.4f\n", GcodePointer.arc_meta.center_pos.x, GcodePointer.arc_meta.center_pos.y);
            feed = GcodePointer.F;
          }
          else
          {
            point_t o = GcodePointer.arc_meta.center_pos;
            point_t p = GcodePointer.arc_meta.last_pos;
            next_point.x = cosf(angle) * (p.x - o.x) - sinf(angle) * (p.y - o.y) + o.x;
            next_point.y = sinf(angle) * (p.x - o.x) + cosf(angle) * (p.y - o.y) + o.y;
            //printf("Arc Tick Tock -> binrep = %d, xo = %0.4f, yo = %0.4f\n", binrep, xo, yo);
            if (GetLineLength(OffsetCordinates, next_point) > (ONE_STEP_DISTANCE * 3)) //Fixes arc follower error without losing arc accuracy
            {
              CNC_BlockingLine(OffsetCordinates, next_point);
            }
            GcodePointer.arc_meta.last_pos = next_point;
            GcodePointer.arc_meta.current_angle = atan2(GcodePointer.arc_meta.center_pos.y - OffsetCordinates.y, GcodePointer.arc_meta.center_pos.x - OffsetCordinates.x);
            //printf("Current Angle: %0.4f, End Angle: %0.4f\n", GcodePointer.arc_meta.current_angle, GcodePointer.arc_meta.end_angle);
            if (InTolerance(GcodePointer.arc_meta.current_angle, GcodePointer.arc_meta.end_angle, ARC_RESOLUTION))
            {
              printf("\t\tReached arc endpoint via end angle!\n");
              MoveDone();
            }
            if (InTolerance(OffsetCordinates.x, GcodePointer.X, (ONE_STEP_DISTANCE + 0.0004)) && InTolerance(OffsetCordinates.y, GcodePointer.Y, (ONE_STEP_DISTANCE + 0.0004)))
            {
              printf("\t\tReached arc endpoint via endpoint!\n");
              MoveDone();
            }
            //printf("X = %0.4f, Y = %0.4f\n", x2, y2);
          }
        }

        //Drive axis to position and set GcodePointer.MoveDone = true;
      }
    }
    else
    {
      //printf("Move Done = true\n");
      //Move is done, read nc file for next instruction, then set GcodePointer.MoveDone = false;
      float X = 0;
      float Y = 0;
      float Z = 0;
      float Xc = 0;
      float Yc = 0;
      float F = 0;
      bool g_valid = false;

      if (getline (nc_file, nc_buffer))
      {
        transform(nc_buffer.begin(), nc_buffer.end(), nc_buffer.begin(), ::toupper);
        printf("NC Line> %s\n", nc_buffer.c_str());


        if (current_file.find(".nc") != std::string::npos)
        {
            vector<string> fields = split(nc_buffer, ' ');
            for(int i = 0; i < fields.size(); i++)
            {
              if (StartsWith(fields[i], "G"))
              {
                fields[i].erase(0, 1); //Remove token
                if (atof(fields[i].c_str()) <= 3) //! valid gcode ! in the future use a lookup table!
                {
                  g_valid = true;
                  GcodePointer.G = atof(fields[i].c_str());
                }
              }
              if (StartsWith(fields[i], "X"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.X = atof(fields[i].c_str());
              }
              if (StartsWith(fields[i], "Y"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.Y = atof(fields[i].c_str());
              }
              if (StartsWith(fields[i], "Z"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.Z = atof(fields[i].c_str());
              }
              if (StartsWith(fields[i], "I"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.I = atof(fields[i].c_str());
              }
              if (StartsWith(fields[i], "J"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.J = atof(fields[i].c_str());
              }
              if (StartsWith(fields[i], "F"))
              {
                fields[i].erase(0, 1); //Remove token
                GcodePointer.F = atof(fields[i].c_str());
              }
            }
            if (g_valid == true)
            {
              GcodePointer.arc_meta.center_pos.x = OffsetCordinates.x + GcodePointer.I;
              GcodePointer.arc_meta.center_pos.y = OffsetCordinates.y + GcodePointer.J;

              GcodePointer.MoveDone = false;
              GcodePointer.FirstInstruction = true;
              g_valid = false;
            }

        }
        else //Handle pgm file
        {
          //Make sure I and J are 0, otherwise it will increment arc meta data center (used in gcode parsing)
          //GcodePointer.I = 0;
          //GcodePointer.J = 0;

          vector<string> fields = split(nc_buffer, ' ');
          if (fields.size() > 4)
          {
            if (fields[1] == "RAPID")
            {
              fields[3].erase(0, 1); //Remove X charactor
              X = atof(fields[3].c_str());

              fields[4].erase(0, 1); //Remove Y charactor
              Y = atof(fields[4].c_str());

              fields[5].erase(0, 1); //Remove Z charactor
              Z = atof(fields[5].c_str());

              printf("\t\tRAPID to X: %0.4f, Y: %0.4f, Z: %0.4f\n", X, Y, Z);

              GcodePointer.G = 0;
              GcodePointer.X = X;
              GcodePointer.Y = Y;
              GcodePointer.Z = Z;
              GcodePointer.MoveDone = false;
              GcodePointer.FirstInstruction = true;

            }
            if (fields[1] == "LINE")
            {
              fields[3].erase(0, 1); //Remove X charactor
              X = atof(fields[3].c_str());

              fields[4].erase(0, 1); //Remove Y charactor
              Y = atof(fields[4].c_str());

              fields[5].erase(0, 1); //Remove Z charactor
              Z = atof(fields[5].c_str());

              fields[6].erase(0, 1); //Remove F charactor
              F = atof(fields[6].c_str());
              printf("\t\tLINE to X: %0.4f, Y: %0.4f, Z: %0.4f at F: %0.4f\n", X, Y, Z, F);

              GcodePointer.G = 1;
              GcodePointer.X = X;
              GcodePointer.Y = Y;
              GcodePointer.Z = Z;
              GcodePointer.F = F;
              GcodePointer.MoveDone = false;
              GcodePointer.FirstInstruction = true;
            }
            if (fields[1] == "ARC|CNTRPT")
            {
              if (fields[3] == "CW")
              {
                fields[4].erase(0, 1); //Remove X charactor
                X = atof(fields[4].c_str());

                fields[5].erase(0, 1); //Remove Y charactor
                Y = atof(fields[5].c_str());

                fields[7].erase(0, 2); //Remove XC charactors
                Xc = atof(fields[7].c_str());

                fields[8].erase(0, 2); //Remove YC charactors
                Yc = atof(fields[8].c_str());

                fields[9].erase(0, 1); //Remove F charactors
                F = atof(fields[9].c_str());

                printf("\t\tClocwise Arc to X: %0.4f, Y: %0.4f with Xc: %0.4f, Yc: %0.4f at F: %0.4f\n", X, Y, Xc, Yc, F);

                GcodePointer.G = 2;
                GcodePointer.X = X;
                GcodePointer.Y = Y;
                GcodePointer.F = F;
                GcodePointer.arc_meta.center_pos.x = Xc;
                GcodePointer.arc_meta.center_pos.y = Yc;
                GcodePointer.MoveDone = false;
                GcodePointer.FirstInstruction = true;
              }
              if (fields[3] == "CCW")
              {
                fields[4].erase(0, 1); //Remove X charactor
                X = atof(fields[4].c_str());

                fields[5].erase(0, 1); //Remove Y charactor
                Y = atof(fields[5].c_str());

                fields[7].erase(0, 2); //Remove XC charactors
                Xc = atof(fields[7].c_str());

                fields[8].erase(0, 2); //Remove YC charactors
                Yc = atof(fields[8].c_str());

                fields[9].erase(0, 1); //Remove F charactor
                F = atof(fields[9].c_str());

                printf("\t\tCounter-Clocwise Arc to X: %0.4f, Y: %0.4f with Xc: %0.4f, Yc: %0.4f at F: %0.4f\n", X, Y, Xc, Yc, F);

                GcodePointer.G = 3;
                GcodePointer.X = X;
                GcodePointer.Y = Y;
                GcodePointer.F = F;
                GcodePointer.arc_meta.center_pos.x = Xc;
                GcodePointer.arc_meta.center_pos.y = Yc;
                GcodePointer.MoveDone = false;
                GcodePointer.FirstInstruction = true;
              }


            }

          }
        }
        if (GcodePointer.Z <= 0 && Cutting == false) //We need to turn cutting head on!
        {
          Cutting = true;
          #ifdef NDEBUG
            digitalWrite(CUTTING_HEAD, Cutting); //CUTTING_HEAD follows cutting logic!
            delay(CUTTING_HEAD_ON_DWELL * 1000);
          #endif
        }
        else if (GcodePointer.Z > 0 && Cutting == true)
        {
          Cutting = false;
          #ifdef NDEBUG
            digitalWrite(CUTTING_HEAD, Cutting); //CUTTING_HEAD follows cutting logic!
            delay(CUTTING_HEAD_OFF_DWELL * 1000);
          #endif
        }
        nc_line++;
      }
    }
  }
  else
  {
    //printf("File is not open!\n");
    GcodePointer.MoveDone = true;
    #ifdef NDEBUG
      delay(500);
    #endif
  }
}
void *cnc_thread(void *p)
{
  while(!quit)
  {
    CNC_Tick();
  }
}
void CNC_Init()
{
  #ifdef NDEBUG
    wiringPiSetup();

    pinMode(CUTTING_HEAD, OUTPUT);
    digitalWrite(CUTTING_HEAD, LOW);
  #endif
  Xaxis = new Stepper(200, X_AXIS);
  Yaxis = new Stepper(200, Y_AXIS);

  //Home maching to establish Machine Cordinates

  MachineCordinates.x = 0;
  MachineCordinates.y = 0;

  OffsetCordinates.x = 0;
  OffsetCordinates.y = 0;

  OffsetValue.x = 0;
  OffsetValue.y = 0;

  GcodePointer.MoveDone = true;

  pthread_t cnc_thread_handle;

  /* create a second thread which executes inc_x(&x) */
  if(pthread_create(&cnc_thread_handle, NULL, cnc_thread, NULL))
  {
    fprintf(stderr, "Error creating CNC thread\n");
  }

}
