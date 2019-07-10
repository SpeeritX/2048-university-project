#pragma once

#include <SDL.h> 
#include <stdio.h>
#include <string.h>
#include "Utilities.h"

enum GameState { MenuState, InGameState, GameOver, RankingMenuState, RankingViewState, LoadGameState };

class Renderer
{
public:
	Renderer(int boardSize, SDL_Renderer &sdl_renderer);
	~Renderer();

	SDL_Texture* loadTexture(char filename[]);
	void renderTileOnBoard(Vector2f pos, int index);
	void renderTileOnBoard(Vector2f pos, int index, float scale);
	void renderTileOnBoard(float x, float y, int index);
	void renderTexture(int x, int y, SDL_Texture* texture);
	void renderTexture(int x, int y, SDL_Texture* texture, int size);
	void renderTexture(Vector2 pos, SDL_Texture* texture, int size);
	void renderTexture(int x, int y, SDL_Texture* texture, int width, int height);
	void renderTime(int x, int y, int time);
	void renderNumber(int x, int y, int num);
	void renderText(int x, int y, char text[]); // Only digits, '.' and ':' and '/' are supported
	void setBackground(GameState state);
	void setBoardSize(int boardSize);
	void renderGameOver();
	void renderGameWon();
	void renderSaveScoreInfo();
	void clear();
	void draw();

private:
	static const int AMOUNT_OF_BG = 6;
	static const int AMOUNT_OF_TILE_TYPES = 12;
	static const int BOARD_SIZE = 432;
	static const int TILE_BORDER = 8;
	const Vector2 DEFAULT_BOARD_POS = Vector2(28, 188);
	static const int LETTER_WIDTH = 20;
	static const int AMOUNT_OF_SYMBOLS = 14;
	static const int FIRST_SYMBOL_INDEX = 45;

	int boardPosX;
	int boardPosY;
	Vector2 boardPos;
	int tileSize;
	SDL_Renderer &sdl_renderer_;
	SDL_Texture *gameOver_;
	SDL_Texture *win_;
	SDL_Texture *toSaveScoreInfo_;
	SDL_Texture *currentBackground_;
	SDL_Texture *backgrounds_[AMOUNT_OF_BG];
	SDL_Texture *tileTextures_[AMOUNT_OF_TILE_TYPES];
	SDL_Texture *symbols_[AMOUNT_OF_SYMBOLS];

	char* toString(int number);
};

