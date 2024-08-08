/*  
 *  Maintainer: Tuka
 *  Last Change: 2024-08-07
*/

/*
 * Todo:
 * Correct the movimentation script(diagonal is running faster than it should, maybe search for some videos to correct it);  
 * Make the whole code better;
 * Draw more sprites;
*/

#include "game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

#define WWIDTH 1366
#define WHEIGHT 768

// moments of the game
typedef enum Gamescreen{TITLE,GB};
static Gamescreen currentscreen;

// main variables
static SDL_Window *win;
static SDL_Renderer *ren;

// + variables
static game cube1;
static game cube2;
static game island1;
static int speedx = 0;
static int speedy = 0;
static int position;
static int maxGld = 200;
static double ang = 0;
static std::vector <game *> gld;

// fps stuff
static int fps = 60;
static int desiredDelta = 1000 / fps;

// title gamescreen config
/*
void title()
{
    SDL_SetRenderDrawColor(ren, 30, 180, 20, 255);

    switch (ev.type)
    {
        
        case SDL_KEYUP:
        {
    
            case SDLK_ENTER:    
            {
                currentscreen=GB;
            }

        }

    }

}
*/

// island scripts
void island1script()
{
    island1.setRenderer(ren);
    island1.loadImage("res/island.png");
    island1.setPos(0, 0);
    island1.setSize(WWIDTH, WHEIGHT);
}

// main object script
void cube()
{
    position = 30;
    cube1.setRenderer(ren);
    cube1.loadImage("res/cube1.png");
    cube1.setPos(position, position);
    cube1.setSize(100, 100);
    cube1.setCenter(150, 150, 37);
}

// main function
int main(void)
{
    win = SDL_CreateWindow("windowshown", 0, 0, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, 0);

    currentscreen=TITLE;

    srand(time(NULL));
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
	    std::cout << "SDL_Init has failed!!. SDL_Error" << SDL_GetError() << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_PNG)))
	{
	    std::cout << "IMG_INIT_PNG HAS FAILED!!! SDL_ERROR: \n" << SDL_GetError() << std::endl;
	}

	if (!(IMG_Init(IMG_INIT_WEBP)))
	{
	    std::cout << "IMG_INIT_WEBP HAS FAILED!!! SDL_ERROR : \n" << SDL_GetError() << std::endl;
	}

	island1script();
    cube();

	bool run = true;
	while(run)
	{
	    int startLoop = SDL_GetTicks();
		SDL_Event ev;

		while (SDL_PollEvent(&ev))
		{
		    switch (ev.type)
			{
                case SDL_QUIT:
				{
				    run = false;
				}
				break;
                
                case GB:
                {
                
                // movement script    
				// key pressing
				    case SDL_KEYDOWN:
				    {

				        switch (ev.key.keysym.sym)
				        {

				            case SDLK_UP:
				            {
                                speedy = -4;
				            }
				            break;

				            case SDLK_LEFT:
				            {
                                speedx = -4;
                            }
				            break;

				            case SDLK_DOWN:
				            { 
                                speedy = 4;
                            }
				            break;

				            case SDLK_RIGHT:
				            {
                                speedx = 4;
				            }
				            break;

				        }

				    }
                    break;
                
                    // key releasing
                    case SDL_KEYUP:
                    {
                    
                        switch (ev.key.keysym.sym)
                        {
                
                            case SDLK_UP:
                            {
                                if ( speedy < 0 )
                                speedy = 0;
                            }

                            case SDLK_LEFT:
                            {
                                if ( speedx < 0 )
                                speedx = 0;
                            }

                            case SDLK_DOWN:
                            {
                                if ( speedy > 0 )
                                speedy = 0;
                            }

                            case SDLK_RIGHT:
                            {
                                if ( speedx > 0 )
                                speedx = 0;
                            }
                
                        }

                    }

                } // for GB

			} // for "switch (ev.type)"
		} // for "while event"

		cube1.move(speedx , speedy);

		SDL_RenderClear(ren);
		island1.draw();
		cube1.draw();

		SDL_RenderPresent(ren);
		SDL_Delay(20);

		int delta = SDL_GetTicks() - startLoop;
		if (delta < desiredDelta)
		{
		    SDL_Delay(desiredDelta - delta);
		}
	} // for "while (run)"

	std::cout << "Program closed!" << std::endl;

	return 0;
}

// game whole conf
void gameInit()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(IMG_INIT_PNG | IMG_INIT_WEBP);
}

void gameQuit()
{
	SDL_Quit();
	IMG_Quit();
}

game::game()
{
	pos.x = 30;
	pos.y = 60;
	image =  NULL;
	pos.w = 100;
	pos.h = 100;
}

game::~game()
{
	if(image != NULL)
	{
		SDL_DestroyTexture(image);
		image = NULL;
	}
}

bool game::loadImage(std::string filename)
{
    if (image != NULL)
    {
        SDL_DestroyTexture(image);
    }
    SDL_Surface *temp = IMG_Load(filename.c_str());
    if(temp != NULL)
    {
        image = SDL_CreateTextureFromSurface(ren, temp);
        SDL_FreeSurface(temp);
        if(image != NULL)
        {
            return false;
        }
    }
    return false;
}

void game::setRenderer(SDL_Renderer *dest)
{
    ren = dest;
}

void game::setSize(int w, int h)
{
    pos.w = w;
    pos.h = h;
}

void game::setPos(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

SDL_Rect *game::getPos()
{
    return &pos;
}

void game::move(int x, int y)
{
    pos.x += x;
    pos.y += y;
}

bool game::getCollision(game * other)
{
    int dx, dy, rs;
    dx = pos.x + center.x - (other->getPos()->x + other->getCenter().x);
    dy = pos.y + center.y - (other->getPos()->y + other->getCenter().y);
    rs = center.r + other->getCenter().r;
    dx *= dx;
    dy *= dy;
    rs *= rs;
    if(dx + dy < rs)
    {
        return true;
    }
    return false;
}

circle game::getCenter()
{
    return center;
}

void game::setCenter(int x, int y, int r)
{
    center.x = x;
    center.y = y;
    center.r = r;
}

void game::draw(double angle)
{
    if(image != NULL)
    {
        SDL_RenderCopyEx(ren, image, NULL, &pos, angle, NULL, SDL_FLIP_NONE);
    }
    else
    {
        std::cout << "Image is NULL at draw!\n" << std::endl;
    }
}

void game::draw()
{
    if(image != NULL)
    {
        SDL_RenderCopy(ren, image, NULL, &pos);
    }
    else
    {
        std::cout << "Image is NULL at draw!\n" << std::endl;
    }
}

