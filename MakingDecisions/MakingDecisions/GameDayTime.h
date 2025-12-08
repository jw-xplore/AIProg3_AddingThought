#pragma once
class GameDayTime
{
public:
	double time = 6.0f;
	int day = 1;

	GameDayTime();
	void updateTime(double dTime, double timeScale);
};

