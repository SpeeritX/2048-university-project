#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>

#include <SDL.h> 
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "Renderer.h"
#include "Game.h"
#include "Ranking.h"

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;
const double FRAME_TIME = 1000 * (1.0 / 60);

const int BOARD_SIZE = 4;


int main(int argc, char **argv)
{
	std::srand(time(NULL));
	bool quit = false;
	double deltaTime = 0;
	Uint64 currentTime = 0;
	Uint64 lastTime = 0;
	SDL_Event event;
	SDL_Window *window;
	SDL_Renderer *sdl_renderer;
	Renderer *renderer;
	Game *game;
	Ranking *ranking;
	GameState state = InGameState;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	}
	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	renderer = new Renderer(BOARD_SIZE, *sdl_renderer);
	game = new Game(BOARD_SIZE, *renderer);
	game->newGame(BOARD_SIZE);
	ranking = new Ranking(*renderer);

	SDL_ShowCursor(SDL_DISABLE);
	currentTime = SDL_GetPerformanceCounter();
	while (!quit)
	{
		lastTime = currentTime;
		currentTime = SDL_GetPerformanceCounter();
		deltaTime += (double)((currentTime - lastTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		if (deltaTime >= FRAME_TIME)
		{
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_UP:
						if (state == InGameState)
							game->move(Up);
						break;
					case SDLK_RIGHT:
						if (state == InGameState)
							game->move(Right);
						else if (state == RankingViewState)
							ranking->nextPage();
						break;
					case SDLK_DOWN:
						if (state == InGameState)
							game->move(Down);
						break;
					case SDLK_LEFT:
						if (state == InGameState)
							game->move(Left);
						else if (state == RankingViewState)
							ranking->prevPage();
						break;
					case SDLK_t:
						if (state == RankingViewState)
							ranking->sortByTime();
						break;
					case SDLK_p:
						if (state == RankingViewState)
							ranking->sortByPoints();
						break;
					case SDLK_u:
						if (state == InGameState)
							game->undoMove();
						break;
					case SDLK_n:
						state = MenuState;
						game->setState(state);
						break;
					case SDLK_r:
						state = RankingMenuState;
						ranking->viewMenu();
						break;
					case SDLK_l:
						state = LoadGameState;
						game->setState(state);
						break;
					case SDLK_s:
						if (state == InGameState)
							game->saveGame();
						else if (state == GameOver)
						{
							Score *score = game->getScore();
							if (score)
							{
								ranking->addScore(score, game->getBoardSize());
								delete score;
							}
							state = MenuState;
							game->setState(MenuState);
						}
						break;
					}

					if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9)
					{
						if (state == MenuState && event.key.keysym.sym >= SDLK_3)
						{
							game->newGame(event.key.keysym.sym - SDLK_0);
							state = InGameState;
						}
						else if (state == RankingMenuState && event.key.keysym.sym >= SDLK_3)
						{
							ranking->viewRanking(event.key.keysym.sym - SDLK_0);
							state = RankingViewState;
						}
						else if (state == LoadGameState)
						{
							if(game->loadGame(event.key.keysym.sym - SDLK_0))
								state = InGameState;
						}
					}

					break;
				case SDL_QUIT:
					quit = 1;
					break;
				}
			}
			renderer->setBackground(state);
			renderer->clear();
			if (state == MenuState || state == InGameState || state == GameOver || state == LoadGameState)
			{
				if (game->update(deltaTime))
					state = GameOver;
			}
			else if (state == RankingMenuState || state == RankingViewState)
			{
				ranking->update();
			}

			renderer->draw();
			deltaTime = 0;
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};
