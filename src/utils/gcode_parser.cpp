#include "gcode_parser.h"
#include "config/handler.h"
#include "utils/terminal.h"
#include "main.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <linux/input.h>
#include <linux/kd.h>

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
        if (gline.find("G") != std::string::npos)
        {

        }

        lines.push_back(g);
    }
    fclose(fp);
  }
}
gcode_t gcode_stack_next()
{

}
