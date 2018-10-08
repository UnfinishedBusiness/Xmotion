#include "javascript.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>


#include "duktape.h"
#include "duk_config.h"
#include "duk_console.h"
#include "duk_module_duktape.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

using namespace std;

duk_context *ctx;

typedef struct
{
    char name[100];
    duk_ret_t (*function)(duk_context*);
}javascript_function_t;

//Private Functions - Javascript Bindings
static duk_ret_t print(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  string p = duk_to_string(ctx, 0);
  printf("%s\n", p.c_str());
	duk_push_number(ctx, 0);
	return 0;  /* one return value */
}
static duk_ret_t get_file(duk_context *ctx)
{
	duk_get_top(ctx);  /* #args */
  string filename(duk_to_string(ctx, 0));
  std::ifstream t(filename);
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);
  //printf("File contents = %s\n", buffer.c_str());
  //buffer = "exports.version = function () { print(\"Hello world from bar!\"); };";
  duk_push_lstring(ctx, (const char *) buffer.c_str(), (duk_size_t) size);
	return 1;  /* one return value */
}
//End Private Functions

javascript_function_t javascript_functions[] = {
  {"print", print},
  {"alert", print},
  {"get_file", get_file},
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

static void push_file_as_string(duk_context *ctx, const char *filename)
{
  std::ifstream t(filename);
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);
  duk_push_lstring(ctx, (const char *) buffer.c_str(), (duk_size_t) size);
}
static void source_file(const char * file)
{
  push_file_as_string(ctx, file);
}
void javascript_modules_init(void)
{
  std::string cmd;
  ctx = duk_create_heap_default();
  duk_console_init(ctx, 0);
  duk_module_duktape_init(ctx);
  registor_functions();

  DIR *dpdf;
  struct dirent *epdf;
  char auto_file[1024];
  dpdf = opendir("javascript/auto");
  if (dpdf != NULL)
  {
    printf("Loading Javascript Modules!\n");
     while (epdf = readdir(dpdf))
     {
        if (strstr(epdf->d_name, ".js") != NULL)
        {
          sprintf(auto_file, "javascript/auto/%s", epdf->d_name);
          printf("\t\tLoading %s\n", auto_file);
          source_file(auto_file);
          if (duk_peval(ctx) != 0) {
              printf("\t\t\tError: %s\n", duk_safe_to_string(ctx, -1));
          }
        }
     }
  }
  closedir(dpdf);
}
void javascript_modules_close(void)
{

}
