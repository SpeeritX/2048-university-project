#include "Utilities.h"


// --------- Vector2f --------------------------------

Vector2f::Vector2f()
	:x(0), y(0)
{
}

Vector2f::Vector2f(int x, int y)
{
	this->x = x;
	this->y = y;
}

Vector2f::Vector2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2f::~Vector2f()
{
}

Vector2f Vector2f::operator+(Vector2f b)
{
	return Vector2f(this->x + b.x, this->y + b.y);
}

Vector2f Vector2f::operator-(Vector2f b)
{
	return Vector2f(this->x - b.x, this->y - b.y);
}

Vector2f Vector2f::operator-(float b)
{
	return Vector2f(this->x - b, this->y - b);
}

Vector2f Vector2f::operator*(float b)
{
	return Vector2f(this->x * b, this->y * b);
}

Vector2f Vector2f::operator*(int b)
{
	return Vector2f(this->x * b, this->y * b);
}

bool Vector2f::operator!=(Vector2f b)
{
	if (this->x == b.x && this->y == b.y)
		return false;

	return true;
}



Vector2::Vector2()
	:x(0), y(0)
{
}

Vector2::Vector2(int x, int y)
{
	this->x = x;
	this->y = y;
}

// --------- Vector2 --------------------------------

Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(Vector2 b)
{
	return Vector2(this->x + b.x, this->y + b.y);
}

Vector2 Vector2::operator-(Vector2 b)
{
	return Vector2(this->x - b.x, this->y - b.y);
}

Vector2 Vector2::operator+(int b)
{
	return Vector2(this->x + b, this->y + b);
}

Vector2 Vector2::operator-(float b)
{
	return Vector2(this->x - b, this->y - b);
}

Vector2 Vector2::operator*(double b)
{
	return Vector2(this->x * b, this->y * b);
}

Vector2 Vector2::operator*(int b)
{
	return Vector2(this->x * b, this->y * b);
}

bool Vector2::operator!=(Vector2 b)
{
	if (this->x == b.x && this->y == b.y)
		return false;

	return true;
}
