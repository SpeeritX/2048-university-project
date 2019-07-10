#include "Game.h"



Game::Game(int boardSize, Renderer &renderer)
	:renderer_(renderer), boardSize_(boardSize), gameState_(MenuState)
{
	board_ = new Board(boardSize, renderer);
	timer_ = new Timer(renderer_);
}


Game::~Game()
{
	delete board_;
	delete timer_;
}

bool Game::update(double dTime)
{
	switch (gameState_)
	{
	case MenuState:
		break;

	case InGameState:
		timer_->update(dTime);
		if (board_->update())
		{
			gameState_ = GameOver;
			return true;
		}
		break;

	case GameOver:
		timer_->update(0);
		board_->update();
		if (board_->isGameWon())
		{
			renderer_.renderGameWon();
			renderer_.renderSaveScoreInfo();
		}
		else
		{
			renderer_.renderGameOver();
			//renderer_.renderSaveScoreInfo();
		}
		break;

	case LoadGameState:
		displaySaves();
		break;
	}
	return false;
}

void Game::move(Direction direction)
{
	board_->move(direction);
}

void Game::undoMove()
{
	board_->undoMove();
}

void Game::setState(GameState state)
{
	gameState_ = state;
	switch (state)
	{
	case MenuState:
		break;

	case InGameState:
		break;

	case LoadGameState:
		getSaves();
		break;
	}
}


bool Game::loadGame(int index)
{
	if (index < saves_.amount && index >= 0)
	{
		newGame(saves_.saves[index].size);
		board_->loadSave(&saves_.saves[index]);
		timer_->setTime(saves_.saves[index].time);
		return true;
	}
	else
		return false;
}

void Game::saveGame()
{
	getSaves();
	Save *newSave = board_->getSave();
	newSave->time = timer_->getTime();
	saves_.pushSave(newSave);
	delete newSave;

	FILE * file;
	file = fopen("save", "wb");
	if (file != NULL)
	{
		fwrite(&saves_, sizeof(struct SaveList), 1, file);
		fclose(file);
	}
}

void Game::newGame(int boardSize)
{
	gameState_ = InGameState;
	boardSize_ = boardSize;
	delete board_;
	board_ = new Board(boardSize, renderer_);
	delete timer_;
	timer_ = new Timer(renderer_);
	renderer_.setBoardSize(boardSize);
}

Score * Game::getScore()
{
	if (!board_->isGameWon())
		return NULL; 

	Score *result = new Score();
	result->points = board_->getPoints();
	result->time = timer_->getTime() / 1000;
	
	return result;

}

int Game::getBoardSize()
{
	return boardSize_;
}

void Game::getSaves()
{
	FILE * file;
	file = fopen("save", "rb");
	if (file != NULL)
	{
		if (!fread(&saves_, sizeof(struct SaveList), 1, file))
			saves_.amount = 0;
		fclose(file);
	}
	else
	{
		saves_.amount = 0;
	}
}

void Game::displaySaves()
{
	char *name = new char[Save::DATE_LENGHT + 3];
	name[0] = '0';
	name[1] = '.';
	name[2] = '-';

	for (int i = 0; i < saves_.amount; ++i)
	{
		name[0] = '0' + i;
		strcpy(&name[3], saves_.saves[i].date);
		renderer_.renderText(FIRST_SAVE_POS_X, FIRST_SAVE_POS_Y + RECORD_HEIGHT * i, name);
	}
	delete[] name;
}
