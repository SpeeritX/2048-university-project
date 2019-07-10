#include "Renderer.h"



Renderer::Renderer(int boardSize, SDL_Renderer &sdl_renderer)
	:sdl_renderer_(sdl_renderer), boardPos(boardPosX, boardPosY)
{
	setBoardSize(boardSize);
	for (int i = 0; i < AMOUNT_OF_BG; ++i)
	{
		char fileName[] = "sprites/bg0.bmp";
		fileName[10] = '0' + i % 10;
		backgrounds_[i] = loadTexture(fileName);
	}
	currentBackground_ = backgrounds_[0];

	for (int i = 0; i < AMOUNT_OF_SYMBOLS; ++i)
	{	
		char fileName[] = "sprites/symbol00.bmp";
		fileName[14] = '0' + (i + FIRST_SYMBOL_INDEX) / 10;
		fileName[15] = '0' + (i + FIRST_SYMBOL_INDEX) % 10;
		symbols_[i] = loadTexture(fileName);
	}

	for (int i = 0; i < AMOUNT_OF_TILE_TYPES; ++i)
	{
		char fileName[] = "sprites/tile00.bmp";
		fileName[12] = '0' + i / 10;
		fileName[13] = '0' + i % 10;
		tileTextures_[i] = loadTexture(fileName);
	}

	char gameOverName[] = "sprites/gameOver.bmp";
	gameOver_ = loadTexture(gameOverName);

	char gameWonName[] = "sprites/gameWon.bmp";
	win_ = loadTexture(gameWonName);

	char scoreInfoName[] = "sprites/toSaveScore.bmp";
	toSaveScoreInfo_ = loadTexture(scoreInfoName);
}


Renderer::~Renderer()
{
	for (int i = 0; i < AMOUNT_OF_BG; ++i)
		SDL_DestroyTexture(backgrounds_[i]);

	for (int i = 0; i < AMOUNT_OF_SYMBOLS; ++i)
		SDL_DestroyTexture(symbols_[i]);

	for (int i = 0; i < AMOUNT_OF_TILE_TYPES; ++i)
		SDL_DestroyTexture(tileTextures_[i]);

	SDL_DestroyTexture(gameOver_);
}

SDL_Texture* Renderer::loadTexture(char filename[])
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedImage = SDL_LoadBMP(filename);

	if (loadedImage != NULL)
	{
		newTexture = SDL_CreateTextureFromSurface(&sdl_renderer_, loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	else
	{
		printf("Error occurred, during loading file: %s\n", filename);
	}

	return newTexture;
}

void Renderer::renderTileOnBoard(Vector2f pos, int index)
{
	renderTileOnBoard(pos.x, pos.y, index);
}

void Renderer::renderTileOnBoard(Vector2f pos, int index, float scale)
{
	pos = pos * tileSize;
	pos = pos - (tileSize - TILE_BORDER)*(scale - 1) / 2;
	renderTexture(pos.x + boardPosX, pos.y + boardPosY, tileTextures_[index], (tileSize - TILE_BORDER)*scale);
}

void Renderer::renderTileOnBoard(float x, float y, int index)
{
	x *= tileSize;
	y *= tileSize;
	renderTexture(x + boardPosX, y + boardPosY, tileTextures_[index], tileSize - TILE_BORDER);
}

void Renderer::renderTexture(int x, int y, SDL_Texture* texture)
{
	renderTexture(x, y, texture, 0, 0);
}

void Renderer::renderTexture(int x, int y, SDL_Texture * texture, int size)
{
	renderTexture(x, y, texture, size, size);
}

void Renderer::renderTexture(Vector2 pos, SDL_Texture * texture, int size)
{
	renderTexture(pos.x, pos.y, texture, size, size);
}

void Renderer::renderTexture(int x, int y, SDL_Texture * texture, int width, int height)
{
	SDL_Rect destination;
	destination.x = x;
	destination.y = y;

	if (width == 0 || height == 0)
		SDL_QueryTexture(texture, NULL, NULL, &destination.w, &destination.h);
	else
		destination.w = width, destination.h = height;

	SDL_RenderCopy(&sdl_renderer_, texture, NULL, &destination);
}

void Renderer::renderTime(int x, int y, int time)
{

	char *minutes = toString(time / 60);
	char *seconds = toString(time - (time / 60) * 60);
	int lenght = 4;
	if (strlen(minutes) > 2)
		lenght += strlen(minutes);
	else
		lenght += 2;
	char *result = new char[lenght];

	if(strlen(minutes) >= 2)
		for (int i = 0; i < strlen(minutes); ++i)
			result[i] = minutes[i];
	else if(strlen(minutes) == 1)
	{
		result[0] = '0';
		result[1] = minutes[0];
	}

	int sepPos = fmax(strlen(minutes), 2);
	result[sepPos] = ':';
	if (strlen(seconds) < 2)
	{
		result[sepPos + 1] = '0';
		result[sepPos + 2] = seconds[0];
	}
	else
	{
		result[sepPos + 1] = seconds[0];
		result[sepPos + 2] = seconds[1];
	}
	result[sepPos + 3] = '\0';
	renderText(x, y, result);
	delete[] minutes;
	delete[] seconds;
	delete[] result;

}

void Renderer::renderNumber(int x, int y, int num)
{
	char *text = toString(num);
	renderText(x, y, text);
	delete[] text;
}

void Renderer::renderText(int x, int y, char text[])
{
	x -= strlen(text)*LETTER_WIDTH / 2;
	for (int i = 0; i < strlen(text); ++i)
	{
		if (!(text[i] >= FIRST_SYMBOL_INDEX && text[i] < FIRST_SYMBOL_INDEX + AMOUNT_OF_SYMBOLS))
		{
			printf("Trying to draw unsupported symbol: %i", text[i]);
		}
		else
		{
			renderTexture(x + LETTER_WIDTH * i, y, symbols_[text[i] - FIRST_SYMBOL_INDEX]);
		}
	}
}

void Renderer::setBackground(GameState state)
{
	currentBackground_ = backgrounds_[(int)state];
}

void Renderer::setBoardSize(int boardSize)
{
	if (boardSize == 3)
	{
		tileSize = BOARD_SIZE / 4;
		boardPosX = DEFAULT_BOARD_POS.x + tileSize / 2;
		boardPosY = DEFAULT_BOARD_POS.y + tileSize / 2;
		boardPos = Vector2(boardPosX, boardPosY);
	}
	else
	{
		tileSize = BOARD_SIZE / boardSize;
		boardPosX = DEFAULT_BOARD_POS.x;
		boardPosY = DEFAULT_BOARD_POS.y;
		boardPos = DEFAULT_BOARD_POS;
	}
}

void Renderer::renderGameOver()
{
	renderTexture(0, 160, gameOver_);
}

void Renderer::renderGameWon()
{
	renderTexture(0, 160, win_);
}

void Renderer::renderSaveScoreInfo()
{
	renderTexture(60, 360, toSaveScoreInfo_);
}

void Renderer::clear()
{
	renderTexture(0, 0, currentBackground_);
}

void Renderer::draw()
{
	SDL_RenderPresent(&sdl_renderer_);
}

char * Renderer::toString(int number)
{
	if (number <= 0)
	{
		char *text = new char[2];
		text[0] = '0';
		text[1] = '\0';
		return text;
	}

	int temp = number;
	int lenght = 0;
	while (temp > 0)
	{
		lenght++;
		temp /= 10;
	}
	char *text = new char[lenght + 1];

	text[lenght] = '\0';

	while (lenght > 0)
	{
		text[--lenght] = '0' + number % 10;
		number /= 10;
	}
	return text;
}
