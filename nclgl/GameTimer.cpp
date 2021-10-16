#include "GameTimer.h"

GameTimer::GameTimer(void)	{
	firstPoint	= std::chrono::high_resolution_clock::now();
	nowPoint	= firstPoint;
	Tick();
}

double	GameTimer::GetTotalTimeSeconds()	const {
	Timepoint time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = time - firstPoint;

	return diff.count();
};

double	GameTimer::GetTotalTimeMSec()		const {
	Timepoint time = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> diff = time - firstPoint;

	return diff.count();
}

void	GameTimer::Tick() {
	Timepoint latestTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> diff = latestTime - nowPoint;

	nowPoint = latestTime;

	timeDelta = diff.count();
}