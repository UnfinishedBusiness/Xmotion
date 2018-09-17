#include "gcode_parser.h"
//#include "config/handler.h"
//#include "utils/terminal.h"
//#include "main.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;
vector<gcode_t> lines;

void gcode_parse(char *file)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  gcode_t g;
  g.x = 0;
  g.y = 0;
  g.z = 0;
  g.i = 0;
  g.j = 0;
  g.k = 0;
  g.r = 0;
  g.f = 0;
  int num_p = 0;
  char number[10];
  fp = fopen(file, "r");
  if (fp == NULL)
  {
    g.status = -1;
  }
  else
  {
    while ((read = getline(&line, &len, fp)) != -1)
    {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        string gline(line, read);
        gline.erase(std::remove(gline.begin(), gline.end(), '\n'), gline.end());
        transform(gline.begin(), gline.end(), gline.begin(), ::toupper);
        //printf("Line: %s\n", gline.c_str());
        g.g_word_counter = 0; //Reset counter on each new line
        g.m_word_counter = 0;
        for (size_t x = 0; x < gline.size(); x++)
        {
          char c = gline[x];
          if (c != ' ') //Ignore all spaces on line
          {
            if (c == '(') //Ignore any line with a comment!
            {
              break;
            }
            if (c == 'G') //Found G word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.g[g.g_word_counter] = atof(number);
              //printf("G[%d] Word - G%0.2f\n", g.g_word_counter, g.g[g.g_word_counter]);
              g.g_word_counter++;
            }
            if (c == 'M') //Found M word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.m[g.m_word_counter] = atof(number);
              //printf("M[%d] Word - M%0.2f\n", g.m_word_counter, g.m[g.m_word_counter]);
              g.m_word_counter++;
            }

            if (c == 'X') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.x = atof(number);
            }
            if (c == 'Y') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.y = atof(number);
            }
            if (c == 'Z') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.z = atof(number);
            }

            if (c == 'I') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.i = atof(number);
            }
            if (c == 'J') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.j = atof(number);
            }
            if (c == 'K') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.k = atof(number);
            }

            if (c == 'R') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.r = atof(number);
            }

            if (c == 'F') //Found X word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.');
              number[num_p-1] = '\0';
              x--;
              g.f = atof(number);
            }
          }
        }
        lines.push_back(g);
    }
    fclose(fp);
  }
}
gcode_t gcode_stack_next()
{
  gcode_t ret;
  return ret;
}
void gcode_stack_dump()
{
  gcode_t g;
  for (size_t x = 0; x < lines.size(); x++)
  {
    g = lines[x];
    printf("Line contains - ");

    if (g.g_word_counter > 0)
    {
      printf("(%d) G Words - ", g.g_word_counter);
      for (int y = 0; y < g.g_word_counter; y++)
      {
        printf("G%0.2f ", g.g[y]);
      }
    }

    if (g.m_word_counter > 0)
    {
      printf("(%d) M Words - ", g.m_word_counter);
      for (int y = 0; y < g.m_word_counter; y++)
      {
        printf("M%0.2f ", g.m[y]);
      }
    }
    printf(" X=%0.4f, Y=%0.4f, Z=%0.4f, I=%0.4f, J=%0.4f, K=%0.4f, R=%0.4f, F=%0.4f", g.x, g.y, g.z, g.i, g.j, g.k, g.r, g.f);
    printf("\n");
  }
}
