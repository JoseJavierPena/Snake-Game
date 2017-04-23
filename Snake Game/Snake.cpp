#define SDL_MAIN_HANDLED
#include <SDL.h>

int WinMain(int, int, char*, int)
{
	SDL_INIT_EVERYTHING;
	SDL_Window* window = SDL_CreateWindow("Snek", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* rederer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event e;

	bool running = true;

	while (running)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type==SDL_QUIT)
			{
				running = false;
			}
			else if(e.type ==SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}
	}

	return 0;
}