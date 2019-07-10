#include "Tile.h"

Tile::Tile(int posX, int posY, Renderer & renderer)
	:position_(posX, posY), used(false), renderer_(renderer), state(ShowingUp)
{
	int newValue = rand() % 8;
	if (newValue == 0)
		value = 2;
	else
		value = 1;
	showingUpAnimation_ = new ShowingUpAnimation(position_, value, renderer_);
}

Tile::Tile(int posX, int posY, int value, Renderer & renderer)
	:position_(posX, posY), used(false), value(value), renderer_(renderer), state(ShowingUp)
{
	showingUpAnimation_ = new ShowingUpAnimation(position_, value, renderer_);
}

Tile::~Tile()
{
	if (showingUpAnimation_)
		delete showingUpAnimation_, showingUpAnimation_ = NULL;

	if (movingAnim[0])
		delete movingAnim[0], movingAnim[0] = NULL;

	if (movingAnim[1])
		delete movingAnim[1], movingAnim[1] = NULL;

	if (mergingAnim_)
		delete mergingAnim_, mergingAnim_ = NULL;
}

void Tile::render()
{
	switch (state)
	{
	case ShowingUp:
		if (showingUpAnimation_->update() == false)
		{
			delete showingUpAnimation_, showingUpAnimation_ = NULL;
			state = Static;
		}
		break;

	case Static:
		renderer_.renderTileOnBoard(position_, value);
		break;

	case Moving:
		if (movingAnim[0])
		{
			if (movingAnim[0]->update() == false)
				state = Static;
		}
		else
			renderer_.renderTileOnBoard(position_, value - 1);

		if (movingAnim[1] && movingAnim[1]->update() == false)
			state = Static;

		if (state == Static)
		{
			if (movingAnim[0])
				delete movingAnim[0], movingAnim[0] = NULL;
			if (movingAnim[1])
			{
				delete movingAnim[1], movingAnim[1] = NULL;
				state = Merging;
				if (mergingAnim_)
					delete mergingAnim_, mergingAnim_ = 0;
				mergingAnim_ = new MergingAnimation(position_, value, renderer_);
			}
		}
		break;

	case Merging:
		if (mergingAnim_->update() == false)
		{
			state = Static;
			delete mergingAnim_, mergingAnim_ = 0;
		}
	}
}

void Tile::moveFrom(int x, int y, int movingValue)
{
	state = Moving;
	if (movingAnim[1])
		delete movingAnim[1], movingAnim[1] = NULL;
	movingAnim[1] = new MovingAnimation(Vector2f(x, y), position_, movingValue, renderer_);
}

void Tile::moveTo(int x, int y)
{
	state = Moving;
	if (movingAnim[0])
		delete movingAnim[0], movingAnim[0] = NULL;
	movingAnim[0] = new MovingAnimation(position_, Vector2f(x, y), value, renderer_);
	position_ = Vector2f(x, y);
}

void Tile::reset(int value)
{
	this->value = value;
	state = Static;
	used = false;
}
