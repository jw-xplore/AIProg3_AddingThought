#pragma once
#include "Buildings.h"
#include "NPCPerson.h"
#include <vector>

/*
World representing buldings and players
*/
class World
{
public:
	std::vector<Building*> buildings;
	std::vector<NPCPerson*> people;

	double time = 21.0f;
	int day = 1;

	World();

	void showPeopleStatus();
	void updateTime(double dTime, double timeScale);
};

