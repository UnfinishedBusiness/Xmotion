#ifndef UTILS_GCODE_PARSER_H
#define UTILS_GCODE_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stddef.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 * VIEWER PROTOTYPES
 **********************/
#define GCODE_COMMENT 1
#define GCODE_SUBROUTINE 2
#define GCODE_NOFILE -1
#define GCODE_MOVE 0
#define GCODE_NONMOVE 3
typedef struct
{
    int n;
    float g[100];
    float m[100];

    float x;
    float y;
    float z;

    float i;
    float j;
    float k;

    float r;

    float f;

    int type;
    int status; //is -1 at end of stack
    int g_word_counter;
    int m_word_counter;
}gcode_t;

typedef struct
{
    int g;
    float x;
    float y;
    //float z; //Viewer is 2D for now
    float i;
    float j;
    //float k; //Viewer is 2D for now
}gcode_move_t;

void gcode_parse(char *);
gcode_t gcode_stack_next();
void gcode_stack_dump();
void gcode_parse_moves();
gcode_move_t gcode_get_move(size_t);
size_t gcode_get_move_count();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*UTILS_GCODE_PARSER_H*/
