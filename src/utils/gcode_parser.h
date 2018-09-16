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
    int g;
    float x;
    float y;
    float z;
    float f;

    int status; //is -1 at end of stack
}gcode_t;

void gcode_parse(char *);
gcode_t gcode_stack_next();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*UTILS_GCODE_PARSER_H*/
