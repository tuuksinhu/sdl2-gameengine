#ifndef GAME
#define GAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include <err.h>

void gameInit();
void gameQuit();

struct circle
{
    int x, y, r;
};

class game
{
	public:
		game();
		virtual ~game();
		void draw();
		void setRenderer(SDL_Renderer * renderer);
		void setSize(int w, int h);
		void draw(double angle);
		void setPos(int x, int y);
		SDL_Rect *getPos();
		void move(int x, int y);
		circle getCenter();
		void setCenter(int x, int y, int r);
		bool getCollision(game* other);
		bool loadImage(std::string filename);

	private:
		SDL_Renderer *ren;
		SDL_Texture *image;
		SDL_Rect pos;
		circle center;
};

#endif
