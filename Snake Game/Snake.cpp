#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdlib>

/*=============================================
 * TO DO:
	 * - MAKE SCORE
	 * - MAKE FOOD NOT SPAWN IN SNAKE
	 * - DIFFICULTY CURVE
	 * - GAME OVER IMAGE
	 * - AFTER EATING FOOD MAKE GAME GO FASTER
 ==============================================*/

const int tail_max = 255;

struct Food
{
	int x, y;

	void move()
	{
		x = rand() % (800 / 16);
		y = rand() % (800 / 16);
	}

	void draw(SDL_Renderer* renderer)
	{
		SDL_Rect r{ 16 * x, 16 * y, 16, 16 };
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &r);
	}
};

struct Vector2
{
	int x, y;
};

Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return Vector2{ a.x + b.x,a.y + b.y };
}

void operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
}

struct Snake
{
	Vector2 pos;
	Vector2 vel;
	uint32_t accumulator;

	Vector2 tail[tail_max];
	int tail_start, tail_end;
	int tail_len;

	void Update(uint32_t delta_Time, Food& food)
	{
		accumulator += delta_Time;

		if (accumulator > (100 - (tail_len / 2)))
		{
			/*=====================
			 * UPDATE EVERY 100 MS
			 =====================*/
			accumulator = 0;

			tail_start++;			
			tail[tail_end] = pos;
			tail_end++;

			pos.x += vel.x;
			pos.y += vel.y;

			/*==========================================
			 * AFTER REACHING THE EDGES THE SNAKE COMES 
			 * BACK FROM THE OPPOSITE SIDE 
			 =========================================*/
			if(pos.x < 0)			
				pos.x = 49;
			
			if(pos.y < 0)
				pos.y = 49;
			
			if(pos.x >= 50)
				pos.x = 0;

			if(pos.y >= 50) 
				pos.y = 50;			

			/*==========================
			 * EAT FOOD, THE FOOD MOVES
			 ==========================*/
			if (pos.x == food.x && pos.y == food.y)
			{
				tail_len += 1;
				tail_start--;
				food.move();
			}

			for (auto i = 0; i < tail_len; i++)
			{
				auto& tail_pos = tail[(tail_start + i) % tail_max];

				if(tail_pos.x == pos.x && tail_pos.y == pos.y)
				{
					/*===============================
					 * WE DIE AFTER COLLAPSING WITH
					 * OUR TAIL
					 ==============================*/
					tail_len = 0;
					tail_start = tail_end;
				}
			}
		}
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for(auto i = 0; i < tail_len; i++)
		{
			auto& tail_pos = tail[(tail_start + i) % tail_max];
			SDL_Rect r{ 16 * tail_pos.x, 16 * tail_pos.y, 16, 16 };
			SDL_RenderFillRect(renderer, &r);
		}
		SDL_Rect r{ 16 * pos.x, 16 * pos.y, 16, 16 };
		SDL_RenderFillRect(renderer, &r);
	}
};

int WinMain(int, int, const char*, int)
{
	SDL_INIT_EVERYTHING;
	auto* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);
	auto* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event e;

	auto running = true;
	Snake snake = {};
	Food food = {};
	food.move();

	uint32_t current_Time = 0;

	while (running)
	{
		auto previous_Time = current_Time;
		current_Time = SDL_GetTicks();
		auto delta_Time = current_Time - previous_Time;

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

				/*==============
				 * KEY UP OR W
				============== */ 
				case SDLK_w:
					if(snake.vel.y != 1)
					{
						snake.vel.y = -1;
						snake.vel.x = 0;						
					}					
					break;

				/*==============
				 * KEY UP OR W
				=== ==========*/
				case SDLK_UP:
					if (snake.vel.y != 1)
					{
						snake.vel.y = -1;
						snake.vel.x = 0;		
					}					
					break;

				/*================
				 * KEY DOWN OR S
				=================*/
				case SDLK_s:
					if (snake.vel.y != -1)
					{
						snake.vel.y = 1;
						snake.vel.x = 0;						
					}
					break;

				/*================
				 * KEY DOWN OR S
				 ================*/
				case SDLK_DOWN:
					if (snake.vel.y != -1)
					{
						snake.vel.y = 1;
						snake.vel.x = 0;					
					}
					break;

				/*================
				 * KEY LEFT OR A
				 ================*/
				case SDLK_a:
					if (snake.vel.x != 1)
					{
						snake.vel.x = -1;
						snake.vel.y = 0;						
					}
					break;

				/*=================
				 * KEY LEFT OR A
				 =================*/
				case SDLK_LEFT:
					if (snake.vel.x != 1)
					{
						snake.vel.x = -1;
						snake.vel.y = 0;						
					}
					break;

				/*=================
				 * KEY RIGHT OR D
				 =================*/
				case SDLK_d:
					if (snake.vel.x != -1)
					{
						snake.vel.x = 1;
						snake.vel.y = 0;						
					}
					break;

				/*=================
				 * KEY RIGHT OR D
				==================*/
				case SDLK_RIGHT:
					if (snake.vel.x != -1)
					{
						snake.vel.x = 1;
						snake.vel.y = 0;					
					}					
					break;

				// Default
				default:
					break;
				}
			}
		}

		snake.Update(delta_Time, food);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		/*=============
		 * GAME HERE
		 =============*/
		snake.Draw(renderer);
		food.draw(renderer);

		SDL_RenderPresent(renderer);
	}
	return 0;
}