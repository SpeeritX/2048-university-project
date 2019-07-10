#pragma once

#include <SDL.h> 
#include "Renderer.h"

class Timer
{
public:
	Timer(Renderer &renderer);
	~Timer();

	void update(double dTime);
	double getTime();
	void setTime(double time);

private:
	const int POS_X = 350;
	const int POS_Y = 120;
	Renderer &renderer_;
	double time_;
};

