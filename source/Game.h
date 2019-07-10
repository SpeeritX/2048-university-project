#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <SDL.h> 
#include <stdio.h>
#include "Board.h"
#include "Renderer.h"
#include "Timer.h"


struct SaveList
{
	Save saves[10];
	int amount = 0;

	void pushSave(Save *save)
	{
		for (int i = 9; i > 0; --i)
			saves[i] = saves[i - 1];
		saves[0] = *save;
		amount++;
		if (amount > 10)
			amount = 10;
	}
};

class Game
{
public:
	Game(int boardSize, Renderer &renderer);
	~Game();

	bool update(double dTime);
	void move(Direction direction);
	void undoMove();
	void setState(GameState state);
	bool loadGame(int index);
	void saveGame();
	void newGame(int boardSize);
	Score* getScore();
	int getBoardSize();

private:
	static const int FIRST_SAVE_POS_X = 240;
	static const int FIRST_SAVE_POS_Y = 200;
	static const int RECORD_HEIGHT = 35;
	Renderer &renderer_;
	Board *board_;
	Timer *timer_;
	SaveList saves_;
	int boardSize_;
	GameState gameState_;

	void getSaves();
	void displaySaves();
};

