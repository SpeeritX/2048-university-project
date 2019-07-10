#include "Timer.h"



Timer::Timer(Renderer &renderer)
	:renderer_(renderer), time_(0)
{
}


Timer::~Timer()
{
}

void Timer::update(double dTime)
{
	time_ += dTime;
	renderer_.renderTime(POS_X, POS_Y, time_ / 1000);
}

double Timer::getTime()
{
	return time_;
}

void Timer::setTime(double time)
{
	time_ = time;
}
