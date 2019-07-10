#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Renderer.h"
#include "Utilities.h"
#include "Tile.h"

enum Direction { Up, Right, Down, Left };

struct Save 
{
	static const int DATE_LENGHT = 20;
	int size;
	int tiles[9][9];
	int prevTiles[9][9];
	char date[DATE_LENGHT];
	bool undo;
	int time;
	int points;
	int prevPoints;
};

class Board
{
public:
	Board(int size, Renderer &renderer);
	~Board();

	bool update();
	void move(Direction direction);
	void undoMove();
	Save* getSave();
	void loadSave(Save *save);
	int getPoints();
	bool isGameWon();

private:
	const int POINTS_POS_X = 130;
	const int POINTS_POS_Y = 120;
	const int WIN_VALUE = 11; // if 2^MIN_VALUE appears, game is won

	int boardSize_;
	Renderer &renderer_;
	Tile ***tiles_;
	int **prevTiles_;
	int points_;
	int prevPoints_;
	bool gameOver_;
	bool gameWon_;

	bool moveTile(int x, int y, Direction dir);
	int getPoints(int value);
	void drawNewTile();
	bool isGameOver();
	void renderBoard();
};

