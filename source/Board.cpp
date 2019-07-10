#include "Board.h"
#include <cstdlib>
#include <cmath>



Board::Board(int size, Renderer &renderer)
	:boardSize_(size), renderer_(renderer), prevTiles_(NULL), points_(0), gameOver_(false), gameWon_(false)
{
	renderer_.setBoardSize(boardSize_);
	tiles_ = new Tile**[size];
	for (int i = 0; i < size; ++i)
	{
		tiles_[i] = new Tile*[size];
		for (int j = 0; j < size; ++j)
			tiles_[i][j] = NULL;
	}

	drawNewTile();
	drawNewTile();
}


Board::~Board()
{
	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_; ++j)
			delete tiles_[i][j];
		delete[] tiles_[i];
	}
	delete[] tiles_;

	if (prevTiles_ != NULL)
	{
		for (int i = 0; i < boardSize_; ++i)
			delete[] prevTiles_[i];
		delete[] prevTiles_;
	}
}

bool Board::update()
{
	renderBoard();
	if (isGameOver() || isGameWon())
		return true;

	return false;
}

void Board::move(Direction direction)
{
	int **prevTemp = new int*[boardSize_];

	for (int i = 0; i < boardSize_; ++i)
	{
		prevTemp[i] = new int[boardSize_];
		for (int j = 0; j < boardSize_; ++j)
			if (tiles_[i][j])
				prevTemp[i][j] = tiles_[i][j]->value;
			else
				prevTemp[i][j] = 0;
	}

	int prevPointsTemp = prevPoints_;
	prevPoints_ = 0;

	bool moved = false;
	for (int i = 0; i < boardSize_; ++i)
		for (int j = 0; j < boardSize_; ++j)
			if (tiles_[j][i]) tiles_[j][i]->used = false;

	if (direction == Up || direction == Left)
	{
		for (int i = 0; i < boardSize_; ++i)
			for (int j = 0; j < boardSize_; ++j)
				if (tiles_[j][i])
					if (moveTile(j, i, direction)) moved = true;
	}
	else
	{
		for (int i = boardSize_ - 1; i >= 0; --i)
			for (int j = boardSize_ - 1; j >= 0; --j)
				if (tiles_[j][i])
					if (moveTile(j, i, direction)) moved = true;
	}

	if (moved)
	{
		drawNewTile();
		if (prevTiles_ != NULL)
		{
			for (int i = 0; i < boardSize_; ++i)
				delete[] prevTiles_[i];
			delete[] prevTiles_;
		}
		prevTiles_ = prevTemp;
		points_ += prevPoints_;
	}
	else
		prevPoints_ = prevPointsTemp;
}

void Board::undoMove()
{
	if (prevTiles_ == NULL)
		return;

	points_ -= prevPoints_;

	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_; ++j)
		{
			if (tiles_[i][j])
				delete tiles_[i][j];
			tiles_[i][j] = NULL;
			if (prevTiles_[i][j])
				tiles_[i][j] = new Tile(i, j, prevTiles_[i][j], renderer_);
		}

		delete prevTiles_[i];
	}
	delete[] prevTiles_;
	prevTiles_ = NULL;
}

Save* Board::getSave()
{
	Save *newSave = new Save();
	newSave->size = boardSize_;
	newSave->points = points_;
	newSave->prevPoints = prevPoints_;

	time_t rawtime;
	struct tm *timeInfo;
	time(&rawtime);
	timeInfo = localtime(&rawtime);
	int year = 1900 + timeInfo->tm_year;
	newSave->date[0] = '0' + year / 1000;
	newSave->date[1] = '0' + year / 100 % 10;
	newSave->date[2] = '0' + year / 10 % 10;
	newSave->date[3] = '0' + year % 10;
	newSave->date[4] = '.';
	newSave->date[5] = '0' + (timeInfo->tm_mon + 1) / 10 % 10;
	newSave->date[6] = '0' + (timeInfo->tm_mon + 1) % 10;
	newSave->date[7] = '.';
	newSave->date[8] = '0' + timeInfo->tm_mday / 10 % 10;
	newSave->date[9] = '0' + timeInfo->tm_mday % 10;
	newSave->date[10] = ' ';
	newSave->date[11] = '0' + timeInfo->tm_hour / 10 % 10;
	newSave->date[12] = '0' + timeInfo->tm_hour % 10;
	newSave->date[13] = ':';
	newSave->date[14] = '0' + timeInfo->tm_min / 10 % 10;
	newSave->date[15] = '0' + timeInfo->tm_min % 10;
	newSave->date[16] = ':';
	newSave->date[17] = '0' + timeInfo->tm_sec / 10 % 10;
	newSave->date[18] = '0' + timeInfo->tm_sec % 10;
	newSave->date[19] = '\0';

	if (prevTiles_)
		newSave->undo = true;
	else
		newSave->undo = false;


	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_; ++j)
			if (tiles_[i][j]) newSave->tiles[i][j] = tiles_[i][j]->value;
			else  newSave->tiles[i][j] = 0;
	}

	if (prevTiles_)
	{
		for (int i = 0; i < boardSize_; ++i)
		{
			for (int j = 0; j < boardSize_; ++j)
				newSave->prevTiles[i][j] = prevTiles_[i][j];
		}
	}


	return newSave;
}

void Board::loadSave(Save *save)
{
	boardSize_ = save->size;
	points_ = save->points;
	prevPoints_ = save->prevPoints;
	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_; ++j)
		{
			if (tiles_[i][j]) delete tiles_[i][j], tiles_[i][j] = NULL;
			if (save->tiles[i][j] > 0)
				tiles_[i][j] = new Tile(i, j, save->tiles[i][j], renderer_);
		}
	}

	if (save->undo)
	{
		if (prevTiles_ == NULL)
		{
			prevTiles_ = new int*[boardSize_];
			for (int i = 0; i < boardSize_; ++i)
				prevTiles_[i] = new int[boardSize_];
		}

		for (int i = 0; i < boardSize_; ++i)
		{
			for (int j = 0; j < boardSize_; ++j)
				prevTiles_[i][j] = save->prevTiles[i][j];
		}
	}
	else if (prevTiles_ != NULL)
	{
		for (int i = 0; i < boardSize_; ++i)
			delete prevTiles_;
		delete[] prevTiles_;
	}
}

int Board::getPoints()
{
	return points_;
}


bool Board::moveTile(int x, int y, Direction dir)
{
	int offsetX = 0;
	int offsetY = 0;

	if (dir == Up) offsetY = -1;
	else if (dir == Right) offsetX = 1;
	else if (dir == Down) offsetY = 1;
	else if (dir == Left) offsetX = -1;

	int newX = x; //coords of a new place for the tile
	int newY = y;

	do
	{
		newX += offsetX;
		newY += offsetY;
	} while (newX >= 0 && newX < boardSize_ && newY >= 0 && newY < boardSize_ && tiles_[newX][newY] == NULL);

	if (!(newX >= 0 && newX < boardSize_ && newY >= 0 && newY < boardSize_) ||			//beyond the board OR
		tiles_[newX][newY]->value != tiles_[x][y]->value || tiles_[newX][newY]->used)	//tiles are different OR new tile is used
	{
		newX -= offsetX;
		newY -= offsetY;
	}

	if(newX == x && newY == y) //new coords and old coords are the same - no move is possible
		return false;
	
	if (tiles_[newX][newY] != NULL) //new tile has the same value
	{
		delete tiles_[x][y];
		tiles_[x][y] = NULL;
		tiles_[newX][newY]->moveFrom(x, y, tiles_[newX][newY]->value);
		tiles_[newX][newY]->value += 1;
		prevPoints_ += getPoints(tiles_[newX][newY]->value);
		tiles_[newX][newY]->used = true;
	}
	else	//new tile is empty
	{
		tiles_[x][y]->moveTo(newX, newY);
		tiles_[newX][newY] = tiles_[x][y];
		tiles_[x][y] = NULL;
	}
	return true;
}

int Board::getPoints(int value)
{
	if(value == 0)
		return 0;

	return pow(2, value);
}

void Board::drawNewTile()
{
	int counter = 0;
	for (int i = 0; i < boardSize_; ++i)
		for (int j = 0; j < boardSize_; ++j)
			if(tiles_[j][i] == NULL)
				++counter;

	if (counter == 0)
		return;

	int newTile = rand() % counter;
	counter = 0;

	for (int i = 0; i < boardSize_; ++i)
		for (int j = 0; j < boardSize_; ++j)
			if (tiles_[j][i] == NULL)
			{
				if (counter == newTile)
				{
					tiles_[j][i] = new Tile(j, i, renderer_);
				}
				++counter;
			}
}

bool Board::isGameOver()
{
	if (gameOver_)
		return true;

	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_ - 1; ++j)
		{
			if (!(tiles_[i][j] && tiles_[i][j + 1] && tiles_[j][i] && tiles_[j + 1][i]))
				return false;

			if (tiles_[i][j]->value == tiles_[i][j + 1]->value)
				return false;

			if (tiles_[j][i]->value == tiles_[j + 1][i]->value)
				return false;
		}
	}

	gameOver_ = true;
	return true;
}

bool Board::isGameWon()
{
	if (gameWon_)
		return true;
	for (int i = 0; i < boardSize_; ++i)
	{
		for (int j = 0; j < boardSize_; ++j)
		{
			if (tiles_[i][j] && tiles_[i][j]->value >= WIN_VALUE)
				gameWon_ = true;
		}
	}
	return false;
}

void Board::renderBoard()
{
	for (int i = 0; i < boardSize_; ++i)
		for (int j = 0; j < boardSize_; ++j)
			renderer_.renderTileOnBoard(i, j, 0);

	for (int i = 0; i < boardSize_; ++i)
		for (int j = 0; j < boardSize_; ++j)
			if (tiles_[i][j])
				tiles_[i][j]->render();

	renderer_.renderNumber(POINTS_POS_X, POINTS_POS_Y, points_);
}
