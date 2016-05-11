#include <CNC.h>

using namespace std;

point_t MachineCordinates;
point_t OffsetCordinates;
point_t OffsetValue;

gcode_t GcodePointer;
bool Hold = true;
bool Stop = false;
Stepper *Xaxis;
Stepper *Yaxis;

ifstream nc_file;
long nc_line = 0;
string nc_buffer;

void CNC_XPlus()
{
  MachineCordinates.x += ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
}
void CNC_XMinus()
{
  MachineCordinates.x -= ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
}
void CNC_YPlus()
{
  MachineCordinates.y += ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
}
void CNC_YMinus()
{
  MachineCordinates.y -= ONE_STEP_DISTANCE;
  OffsetCordinates.x = MachineCordinates.x - OffsetValue.x;
  OffsetCordinates.y = MachineCordinates.y - OffsetValue.y;
}

void CNC_JogXPlus()
{
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(+1);
  CNC_XPlus();
}
void CNC_JogXMinus()
{
  Xaxis->SetFeedRate(RAPID_FEED);
  Xaxis->Step(-1);
  CNC_XMinus();
}
void CNC_JogYPlus()
{
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(+1);
  CNC_YPlus();
}
void CNC_JogYMinus()
{
  Yaxis->SetFeedRate(RAPID_FEED);
  Yaxis->Step(-1);
  CNC_YMinus();
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
  Hold = false;
  printf("Start!\n");
}
void CNC_SetOrigin()
{
  //printf("Setting Origin!\n");
  OffsetValue.x = MachineCordinates.x;
  OffsetValue.y = MachineCordinates.y;
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
void CNC_Tick()
{
  if (nc_file.is_open())
  {
    if (GcodePointer.MoveDone == false)
    {
      //printf("Move Done = false\n");
      if (Hold == false)
      {
        if (GcodePointer.G == 0)
        {
          if ((OffsetCordinates.x - GcodePointer.X) < 0)
          {
            Xaxis->SetFeedRate(RAPID_FEED);
            Xaxis->Step(+1);
            CNC_XPlus();
          }
          if ((OffsetCordinates.x - GcodePointer.X) > 0)
          {
            Xaxis->SetFeedRate(RAPID_FEED);
            Xaxis->Step(-1);
            CNC_XMinus();
          }

          if (InTolerance(OffsetCordinates.x, GcodePointer.X, ONE_STEP_DISTANCE))
          {
            printf("Reached endpoint!\n");
            GcodePointer.MoveDone = true;
          }
        }
        //Drive axis to position and set GcodePointer.MoveDone = true;
      }
    }
    else
    {
      //printf("Move Done = true\n");
      //Move is done, read nc file for next instruction, then set GcodePointer.MoveDone = false;
      if (getline (nc_file, nc_buffer) == 0)
      {
        printf("End of file!\n");
        nc_file.close(); //End of file!
      }
      else
      {
        printf("NC Line> %s\n", nc_buffer.c_str());

        string number = "";
        float val = 0;
        string letter = "";
        string::size_type sz;
        for (int i = 0; i < nc_buffer.size(); i++)
        {
          if (nc_buffer[i] == 'G' || nc_buffer[i] == 'X' || nc_buffer[i] == 'Y' || nc_buffer[i] == 'Z' || nc_buffer[i] == 'F')
          {
            letter.push_back(nc_buffer[i]);
            while(i < nc_buffer.size())
            {
              if (nc_buffer[i+1] != '.' && isalpha(nc_buffer[i+1]))
              {
                break;
              }
              number.push_back(nc_buffer[i+1]);
              i++;
            }
            val = stof(number, &sz);

            //printf("Letter: %s, Value: %0.6f\n", letter.c_str(), val);
            if (letter == "G")
            {
              GcodePointer.G = val;
            }
            if (letter == "X")
            {
              GcodePointer.X = val;
            }
            if (letter == "Y")
            {
              GcodePointer.Y = val;
            }
            if (letter == "Z")
            {
              GcodePointer.Z = val;
            }
            if (letter == "F")
            {
              GcodePointer.F = val;
            }

            number = "";
            letter = "";
          }
        }

        GcodePointer.MoveDone = false;
        nc_line++;
      }
    }
  }
  else
  {
    printf("File is not open!\n");
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
  wiringPiSetup();
  Xaxis = new Stepper(200, 29, 28, 27, 26);
  Yaxis = new Stepper(200, 29, 28, 27, 26);

  //Home maching to establish Machine Cordinates

  MachineCordinates.x = 0;
  MachineCordinates.y = 0;

  OffsetCordinates.x = 0;
  OffsetCordinates.y = 0;

  OffsetValue.x = 0;
  OffsetValue.y = 0;

  GcodePointer.MoveDone = true;

  nc_file.open("test.nc");

  pthread_t cnc_thread_handle;

  /* create a second thread which executes inc_x(&x) */
  if(pthread_create(&cnc_thread_handle, NULL, cnc_thread, NULL))
  {
    fprintf(stderr, "Error creating CNC thread\n");
  }

}
