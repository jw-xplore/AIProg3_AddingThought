#include "World.h"
#include <iostream>

World::World()
{
	// Buildings
	Building* home1 = new Building("House 001");
	Workplace* factory = new Workplace("Factory", 2, 0.1, 7, 12, 15);

	buildings.push_back(home1);
	buildings.push_back(factory);
	buildings.push_back(new Diner("Buffet", 2, 0.3, 0.05));

	// People
	people.push_back(new NPCPerson("Bjorn Bergstrom", this, 10, home1, factory));
}

/*
Show full state of each person, including where and what are they doing
*/
void World::showPeopleStatus()
{
	for (int i = 0; i < people.size(); i++)
	{
		NPCResources* rsc = &people[i]->resources;

		std::cout << people[i]->name << std::endl;
		std::cout << "Hunger: " << rsc->stomachLevel << ", Energy: " << rsc->sleepLevel << ", Money: " << rsc->money << std::endl;
		std::cout << "Doing: " << people[i]->actionName(people[i]->currentAction) << ", At: " << people[i]->currentPlace->name << std::endl;
	}
}

void World::updateTime(double dTime, double timeScale)
{
	float prevTime = time;

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

	// Update people
	for (int i = 0; i < people.size(); i++)
	{
		people[i]->followSchedule(time, prevTime);
	}
}