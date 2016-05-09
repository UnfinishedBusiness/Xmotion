#ifndef CONFIG_
#define CONFIG_
#include <application.h>

void Config_Init();
void Config_LoadScripts();
void Config_RunScript(std::string);
int GetByTag(std::string);
#endif
