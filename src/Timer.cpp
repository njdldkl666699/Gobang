#include"Timer.h"

Timer::Timer() :dura(0.0) {};

void Timer::start()
{
	stampBeg = clock::now();
}

void Timer::pause()
{
	stampEnd = clock::now();
	dura += Duration(stampEnd - stampBeg).count();
}

void Timer::restore()
{
	stampBeg = clock::now();
}

void Timer::stop()
{
	stampEnd = clock::now();
	dura += Duration(stampEnd - stampBeg).count();
}

double Timer::duration() const
{
	return dura;
}