#include <Render.h>

using namespace std;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Current displayed texture
SDL_Texture* gTexture = NULL;

color_t BackgroundColor = color_t{ 0, 0, 0, 0 }; //Default Black

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;

bool Render_Init()
{
  //Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		if (sim)
		{
			gWindow = SDL_CreateWindow( APPLICATION_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		}
		else
		{
			gWindow = SDL_CreateWindow( APPLICATION_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN|SDL_WINDOW_OPENGL );
		}
		
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}
SDL_Texture* Render_loadImage( string path )
{
  //The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}
SDL_Texture* Render_RotateSurface(SDL_Surface* Surface, double Angle)
{
  return NULL;
}
void Render_RenderStack()
{
    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, BackgroundColor.a );

		sort( ObjectStack.begin( ), ObjectStack.end( ), [ ]( const object_t& lhs, const object_t& rhs )
		{
   		return lhs.zindex < rhs.zindex;
		});
    SDL_Rect dst;
    SDL_RenderClear( gRenderer );
    for (int x = 0; x < ObjectStack.size(); x++)
    {
      if (ObjectStack[x].visable == true)
      {
        //Render texture to screen
  	    dst.x = ObjectStack[x].position.x;
  	    dst.y = ObjectStack[x].position.y;
  	    //Query the texture to get its width and height to use
        if (ObjectStack[x].size.w > 0 && ObjectStack[x].size.h > 0)
        {
          dst.w = ObjectStack[x].size.w;
          dst.h = ObjectStack[x].size.h;
        }
        else
        {
          SDL_QueryTexture(ObjectStack[x].texture, NULL, NULL, &dst.w, &dst.h);
        }
				SDL_SetTextureBlendMode( ObjectStack[x].texture, SDL_BLENDMODE_BLEND );
				SDL_SetTextureAlphaMod( ObjectStack[x].texture, ObjectStack[x].alpha );
        SDL_RenderCopyEx( gRenderer, ObjectStack[x].texture, NULL, &dst, ObjectStack[x].angle, NULL, SDL_FLIP_NONE );
      }
    }
    //Update screen
    SDL_RenderPresent( gRenderer );
}
void Render_Close()
{
  for (int x = 0; x < ObjectStack.size(); x++)
  {
    SDL_DestroyTexture( ObjectStack[x].texture );
  	ObjectStack[x].texture = NULL;
  }
  SDL_DestroyRenderer( gRenderer );
  gRenderer = NULL;
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
