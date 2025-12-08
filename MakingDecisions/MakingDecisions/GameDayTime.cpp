#include "GameDayTime.h"

GameDayTime::GameDayTime()
{

}

void GameDayTime::updateTime(double dTime, double timeScale)
{
	// Progress time
	time += dTime * timeScale;
	if (time - (int)time > 0.6f)
	{
		time = (int)time + 1 + (time - (int)time - 0.6f);
	}

	// Progress days
	if (time > 24)
	{
		time = time - 24.0f;
		day++;
	}
}