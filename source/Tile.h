#pragma once

#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include "Renderer.h"
#include "Utilities.h"

enum TileState { ShowingUp, Static, Moving, Merging };

class ShowingUpAnimation
{
public:
	ShowingUpAnimation(Vector2f position, int value, Renderer &renderer)
		:position_(position), value_(value), renderer_(renderer), scale_(0)
	{
	}

	bool update()
	{
		scale_ += scaleSpeed_;
		if (scale_ >= 1)
		{
			scaleSpeed_ = 0;
			scale_ = 1;
		}

		renderer_.renderTileOnBoard(position_, value_, scale_);
		if (scale_ == 1)
			return false;
		return true;
	}

private:
	Vector2f position_;
	int value_;
	float scale_;
	float scaleSpeed_ = 0.1;
	Renderer &renderer_;
};

class MergingAnimation
{
public:
	MergingAnimation(Vector2f position, int value, Renderer &renderer)
		:position_(position), value_(value), renderer_(renderer), scale_(1), growing_(true)
	{
	}

	bool update()
	{
		if (growing_ == true)
		{
			scale_ += scaleSpeed_;
			if (scale_ >= 1.15)
				growing_ = false;
		}
		else
		{
			scale_ -= scaleSpeed_;
			if (scale_ <= 1)
			{
				scale_ = 0;
				renderer_.renderTileOnBoard(position_, value_, 1);
				return false;
			}
		}
		renderer_.renderTileOnBoard(position_, value_, scale_);
		return true;
	}

private:
	Vector2f position_;
	int value_;
	float scale_;
	float scaleSpeed_ = 0.05;
	bool growing_;
	Renderer &renderer_;
};

class MovingAnimation
{
public:
	MovingAnimation(Vector2f startPos, Vector2f destination, int value, Renderer &renderer)
		:position_(startPos), destination_(destination), value_(value), renderer_(renderer)
	{
		int dist = destination_.x - position_.x + destination_.y - position_.y;
		if (abs(dist) > 0)
		{
			animSpeed_ = 0.1 * dist;//pow(MIN_DISTANCE / (distance_.x + distance_.y), 1.0 / ANIM_DURATION);
		}
	}

	~MovingAnimation() {}

	bool update()
	{
		if (position_.x != destination_.x)
		{
			position_.x += animSpeed_;
			if (abs(position_.x - destination_.x) <= abs(2 * animSpeed_))
				position_ = destination_;
		}
		else if (position_.y != destination_.y)
		{
			position_.y += animSpeed_;
			if (abs(position_.y - destination_.y) <= abs(2 * animSpeed_))
				position_ = destination_;
		}

		renderer_.renderTileOnBoard(position_, value_);

		if (position_.x == destination_.x && position_.y == destination_.y)
		{
			animSpeed_ = 0;
			return false;
		}
		return true;
	}

private:
	Vector2f destination_;
	Vector2f position_;
	double animSpeed_;
	int value_;
	Renderer &renderer_;
};


class Tile
{
public:
	bool used;
	int value;
	TileState state;

	Tile(int posX, int posY, Renderer &renderer);
	Tile(int posX, int posY, int value, Renderer &renderer);

	~Tile();

	void render();
	void moveFrom(int x, int y, int movingValue);
	void moveTo(int x, int y);
	void reset(int value);
	

private:
	Renderer &renderer_;
	Vector2f position_;
	MovingAnimation *movingAnim[2] = { NULL, NULL };
	MergingAnimation *mergingAnim_ = NULL;
	ShowingUpAnimation * showingUpAnimation_ = NULL;

};
