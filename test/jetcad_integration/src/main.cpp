#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


#include "duktape.h"
#include "duk_config.h"
#include "duk_console.h"
#include "duk_module_duktape.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;
duk_context *ctx;

typedef struct
{
    char name[100];
    duk_ret_t (*function)(duk_context*);
}javascript_function_t;

static duk_ret_t print(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  string p = duk_to_string(ctx, 0);
  printf("%s\n", p.c_str());
	duk_push_number(ctx, 0);
	return 0;  /* one return value */
}

javascript_function_t javascript_functions[] = {
  {"print", print},
  {"alert", print},
  {" ", NULL}
};

void registor_functions()
{
  int x = 0;
  do
  {
    duk_push_global_object(ctx);
  	duk_push_c_function(ctx, javascript_functions[x].function, DUK_VARARGS);
  	duk_put_prop_string(ctx, -2, javascript_functions[x].name);
  	duk_pop(ctx);
    x++;
  }while(strcmp(javascript_functions[x].name, " "));
}

int main(void)
{
  std::string cmd;
  ctx = duk_create_heap_default();
  duk_console_init(ctx, 0);

  duk_module_duktape_init(ctx);
  //printf("top after init: %ld\n", (long) duk_get_top(ctx));
  registor_functions();
  if (ctx)
  {
    while(1)
    {
      cout << "> ";
      getline(cin, cmd);
      cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());
      if (cmd == "exit")
      {
          duk_destroy_heap(ctx);
          break;
      }
      duk_push_string(ctx, cmd.c_str());
      if (duk_peval(ctx) != 0)
      {
        printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
      }
      else
      {
        if (!strcmp(duk_safe_to_string(ctx, -1), "undefined"))
        {
          printf("result is: %s\n", duk_safe_to_string(ctx, -1));
        }
      }
      duk_pop(ctx);  /* pop result */
    }
  }
  return 0;
}
