#include <application.h>

using namespace std;

std::vector<object_t> ObjectStack;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
long RenderTimer = 0;
bool sim = false;
bool quit = false;

void ctrl_c_handler(int s)
{
    printf("Bye!\n");
    quit = true; //Shutdown clean!
}

int main( int argc, char* argv[] )
{
  signal (SIGINT,ctrl_c_handler);
  //
  if (argc > 1)
  {
    if (strcmp(argv[1], "-sim") == 0)
    {
      printf("Starting in simulator mode!\n");
      sim = true;
    }
  }
	//Start up SDL and create window
	if( !Render_Init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
    if (!sim) SDL_ShowCursor(0);
		Config_Init();
    CNC_Init();
    if (sim == false)
		{
			if (Serial_Init() < 0)
			{
				Render_Close();
				exit(1);
			}
		}
		SDL_Event e;
		//While application is running
		while( !quit )
		{
      if ((RenderTimer + 50) < millis())
      {
        RenderTimer = millis();
  			//Handle events on queue
  			while( SDL_PollEvent( &e ) != 0 )
  			{
  				//User requests quit
  				if( e.type == SDL_QUIT )
  				{
  					quit = true;
  				}
  				if (e.type == SDL_KEYDOWN)
  				{
  					switch( e.key.keysym.sym )
        		{
        			case SDLK_ESCAPE: quit = true; break;
  						case SDLK_SPACE: Config_Init(); break;
        		}
  				}
          if (e.type == SDL_MOUSEMOTION )
          {
            //int x = 0, y = 0;
            //SDL_GetMouseState(&x, &y);
            //printf("Move: %d,%d\n", x, y);
          }
          if( e.type == SDL_MOUSEBUTTONDOWN )
          {
            //If the left mouse button was pressed
            if( e.button.button == SDL_BUTTON_LEFT )
            {
                //int mx = e.tfinger.x * SCREEN_WIDTH;
                //int my = e.tfinger.y * SCREEN_HEIGHT;

                //Doesnt work on pi sdl2-2.0.3 but works with sdl2-2.0.4 but with a wierd number set, like 100 - 4020 on x and y
                int mx, my;
                if (sim)
                {
                  mx = e.button.x;
                  my = e.button.y;
                }
                else
                {
                  mx = map_range(e.button.x, 10, 4020, 0, SCREEN_WIDTH);
                  my = map_range(e.button.y, 10, 4020, 0, SCREEN_HEIGHT);
                }

                /*if (mx > 800 && my > 4000)
                {
                  printf("Powering down!");
                  Render_Close();
                  system("poweroff");
                }*/
                //Doesnt work on PI
                //int mx, my;
                //SDL_GetMouseState(&mx, &my);

                //printf("Clicked at %d,%d\n", mx , my);
                string clicked = "";
                for(int x = 0; x < ObjectStack.size(); x++)
                {
                  if (ObjectStack[x].type == INPUT && ObjectStack[x].visable == true)
                  //if (ObjectStack[x].visable == true)
                  {
                    if( ( mx > ObjectStack[x].position.x ) && ( mx < ObjectStack[x].position.x + ObjectStack[x].size.w  ) &&
                        ( my > ObjectStack[x].position.y ) && ( my < ObjectStack[x].position.y + ObjectStack[x].size.h ) )
                    {
                      //printf("Clicked Tag -> %s\n", ObjectStack[x].tagname.c_str());
                      clicked = ObjectStack[x].tagname;
                    }
                    //printf("Tagname - > %s\n", ObjectStack[x].tagname.c_str());
                    //printf("\tposition.x = %d\n", ObjectStack[x].position.x);
                    //printf("\tposition.y = %d\n", ObjectStack[x].position.y);
                    //printf("\tsize.w = %d\n", ObjectStack[x].size.w);
                    //printf("\tsize.h = %d\n", ObjectStack[x].size.h);
                  }
                }
                for(int x = 0; x < ObjectStack.size(); x++)
                {
                  if (ObjectStack[x].tagname == clicked)
                  {
                    if (ObjectStack[x].tagname == "SetOrigin")
                    {
                      CNC_SetOrigin();
                    }
                    else if (ObjectStack[x].tagname == "Stop")
                    {
                      CNC_Stop();
                    }
                    else if (ObjectStack[x].tagname == "Start")
                    {
                      CNC_Start();
                    }
                    else if (ObjectStack[x].tagname == "Hold")
                    {
                      CNC_Hold();
                    }
                    else
                    {
                      //printf("Clicked Tag -> %s at index %d\n", ObjectStack[x].tagname.c_str(), x);
                      while(1) //Should add breakout timer so we cant hang!
                      {
                        SDL_PollEvent( &e );
                        if( e.type == SDL_MOUSEBUTTONUP )
                        {
                          //printf("Button Release!\n");
                          break;
                        }
                        //printf("Waiting for release!\n");
                        if (ObjectStack[x].tagname == "JogXPlus")
                        {
                          CNC_JogXPlus();
                        }
                        if (ObjectStack[x].tagname == "JogXMinus")
                        {
                          CNC_JogXMinus();
                        }
                        if (ObjectStack[x].tagname == "JogYPlus")
                        {
                          CNC_JogYPlus();
                        }
                        if (ObjectStack[x].tagname == "JogYMinus")
                        {
                          CNC_JogYMinus();
                        }
                      }
                    }

                    //Config_RunScript(ObjectStack[x].tagname);
                    break; //Only run one function!
                  }
                }

              }
            }
  			}
  			if (sim == false)
        {
          Serial_Parse();
        }
        else
        {
          Render_RenderStack();
        }
      }
      CNC_Tick();
		}
	}

	//Free resources and close SDL
	Render_Close();
	if (sim == false) Serial_Close();
	return 0;
}
