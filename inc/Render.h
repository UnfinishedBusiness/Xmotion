#ifndef RENDER_
#define RENDER_
#include <application.h>

bool Render_Init();
SDL_Texture* Render_loadImage(std::string);
//bool Render_LoadImage(std::string);
void Render_RenderStack();
void Render_Close();
#endif
