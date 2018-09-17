#ifndef UTILS_GCODE_PARSER_H
#define UTILS_GCODE_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

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

void gcode_parse(char *);
gcode_t gcode_stack_next();
void gcode_stack_dump();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*UTILS_GCODE_PARSER_H*/
