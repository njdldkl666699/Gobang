#pragma once
#include"libs.h"

class Timer
{
	using clock = std::chrono::steady_clock;
	using stamp = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::duration<double>;

public:
	Timer();

	void start();

	void pause();

	void restore();

	void stop();

	double duration() const;

private:
	stamp stampBeg;
	stamp stampEnd;
	double dura;
};

