#include "utils/gcode_parser.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    gcode_parse("0.ngc");
    //gcode_stack_dump();
    gcode_parse_moves();

    gcode_move_t move;
    for (size_t x = 0; x < gcode_get_move_count(); x++)
    {
      move = gcode_get_move(x);
      if (move.g == 0 ) printf("Rapid move to X%0.4f Y%0.4f\n", move.x, move.y);
      if (move.g == 1 ) printf("Line move to X%0.4f Y%0.4f\n", move.x, move.y);
      if (move.g == 2 ) printf("Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
      if (move.g == 3 ) printf("Clockwise arc move to X%0.4f Y%0.4f I%0.4f J%0.4f\n", move.x, move.y, move.i, move.j);
    }
    return 0;
}
