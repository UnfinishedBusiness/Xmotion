#ifndef JAVASCRIPT_
#define JAVASCRIPT_

#include "duktape.h"

extern duk_context *ctx;
void javascript_modules_init(void);
void javascript_modules_close(void);
void javascript_modules_eval(const char*);
#endif
