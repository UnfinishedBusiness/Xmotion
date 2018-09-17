#include "utils/gcode_parser.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    gcode_parse("0.ngc");
    gcode_stack_dump();
    return 0;
}
