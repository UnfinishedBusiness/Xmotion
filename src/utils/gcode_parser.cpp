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
vector<gcode_move_t> moves;

size_t move_count;
size_t current_move;

void gcode_stack_clear()
{
  lines.clear();
  moves.clear();
}
void gcode_parse(const char *file)
{
  gcode_stack_clear();
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
        g.x_set = false;
        g.y_set = false;
        g.z_set = false;
        g.i_set = false;
        g.j_set = false;
        g.k_set = false;
        g.r_set = false;
        g.f_set = false;
        g.status = GCODE_MOVE;
        for (size_t x = 0; x < gline.size(); x++)
        {
          char c = gline[x];
          if (c != ' ') //Ignore all spaces on line
          {
            if (c == '(') //Ignore any line with a comment
            {
              g.status = GCODE_COMMENT;
              break;
            }
            if (c == 'O') //Ignore any line with a comment
            {
              g.status = GCODE_SUBROUTINE;
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
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
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
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.m[g.m_word_counter] = atof(number);
              //printf("M[%d] Word - M%0.2f\n", g.m_word_counter, g.m[g.m_word_counter]);
              g.m_word_counter++;
            }

            if (c == 'X') //Found X word!
            {
              g.x_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.x = atof(number);
            }
            if (c == 'Y') //Found Y word!
            {
              g.y_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.y = atof(number);
            }
            if (c == 'Z') //Found Z word!
            {
              g.z_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.z = atof(number);
            }

            if (c == 'I') //Found I word!
            {
              g.i_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.i = atof(number);
            }
            if (c == 'J') //Found J word!
            {
              g.j_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.j = atof(number);
            }
            if (c == 'K') //Found K word!
            {
              g.k_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.k = atof(number);
            }

            if (c == 'R') //Found R word!
            {
              g.r_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.r = atof(number);
            }

            if (c == 'F') //Found F word!
            {
              g.f_set = true;
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.f = atof(number);
            }

            if (c == 'N') //Found N word!
            {
              number[0] = '\0';
              num_p = 0;
              do{
                x++;
                number[num_p] = gline[x];
                num_p++;
              }while(isdigit(gline[x]) || gline[x] == '.' || gline[x] == '-');
              number[num_p-1] = '\0';
              x--;
              g.n = atoi(number);
            }
          }
        }
        if (g.x_set != false || g.y_set != false || g.z_set != false || g.i_set != false || g.j_set != false || g.k_set != false || g.r_set != false || g.f_set != false)
        {
          lines.push_back(g);
        }
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
void gcode_parse_moves()
{
  gcode_t last_g;
  gcode_t g;
  int current_move = -1;
  for (size_t x = 0; x < lines.size(); x++)
  {
    g = lines[x];
    if (g.status == GCODE_MOVE)
    {
      if (g.g_word_counter > 0)
      {
        //printf("(%d) G Words - ", g.g_word_counter);
        for (int y = 0; y < g.g_word_counter; y++)
        {
          //printf("G%0.2f ", g.g[y]);
          if (g.g[y] == 0 || g.g[y] == 1 || g.g[y] == 2 || g.g[y] == 3)
          {
            current_move = g.g[y];
          }
          else
          {
            current_move = -1;
          }
        }
      }
      //if (last_g.x == g.x && last_g.y == g.y && last_g.z == g.z) //Last move is the same as this move, likly a f word set
      //{
        //current_move = -1;
      //}
      if (current_move == 0) //Rapid move
      {
        //printf("Rapid move to X%0.4f Y%0.4f Z%0.4f\n", g.x, g.y, g.z);
        //if (last_g.z != last_g.z)
        //{
          gcode_move_t move;
          move.g = 0;
          move.x = g.x;
          move.y = g.y;
          moves.push_back(move);
        //}
      }
      else if (current_move == 1) //Line move
      {
        //printf("Line move to X%0.4f Y%0.4f Z%0.4f F%0.4f\n", g.x, g.y, g.z, g.f);
        gcode_move_t move;
        move.g = 1;
        move.x = g.x;
        move.y = g.y;
        moves.push_back(move);
      }
      else if (current_move == 2 && g.k_set == false) //Clockwise arc, don't include arcs on XZ or YZ planes
      {
        //printf("Clockwise arc move to X%0.4f Y%0.4f Z%0.4f I%0.4f J%0.4f K%0.4f F%0.4f\n", g.x, g.y, g.z, g.i, g.j, g.k, g.f);
        gcode_move_t move;
        move.g = 2;
        move.x = g.x;
        move.y = g.y;
        move.i = g.i;
        move.j = g.j;
        moves.push_back(move);
      }
      else if (current_move == 3 && g.k_set == false) //Counter-Clockwise arc, don't include arcs on XZ or YZ planes
      {
        //printf("Counter-Clockwise arc move to X%0.4f Y%0.4f Z%0.4f I%0.4f J%0.4f K%0.4f F%0.4f\n", g.x, g.y, g.z, g.i, g.j, g.k, g.f);
        gcode_move_t move;
        move.g = 3;
        move.x = g.x;
        move.y = g.y;
        move.i = g.i;
        move.j = g.j;
        moves.push_back(move);
      }
    }
    last_g = g;
  }
  move_count = moves.size();
}
size_t gcode_get_move_count()
{
  return move_count;
}
gcode_move_t gcode_get_move(size_t i)
{
  return moves.at(i);
}
