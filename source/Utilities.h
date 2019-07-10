#pragma once
class Vector2f
{
public:
	Vector2f();
	Vector2f(int x, int y);
	Vector2f(float x, float y);
	~Vector2f();

	Vector2f operator+(Vector2f b);
	Vector2f operator-(Vector2f b);
	Vector2f operator-(float b);
	Vector2f operator*(float b);
	Vector2f operator*(int b);
	bool operator!=(Vector2f b);


	float x;
	float y;
};

class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	~Vector2();

	Vector2 operator+(Vector2 b);
	Vector2 operator-(Vector2 b);
	Vector2 operator+(int b);
	Vector2 operator-(float b);
	Vector2 operator*(double b);
	Vector2 operator*(int b);
	bool operator!=(Vector2 b);


	int x;
	int y;
};


struct Score
{
	int points;
	int time;
};