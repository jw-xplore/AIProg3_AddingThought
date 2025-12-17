#include "World.h"
#include <iostream>
#include "TreeManager.h"
#include <string>

World::World()
{
	//treeManager = new TreeManager();

	// Items
	items.push_back(Item("Coffee", 10));
	items.push_back(Item("Kanelbulle", 20));
	items.push_back(Item("Vasterbottensost", 100));
	items.push_back(Item("Snus", 150));
	items.push_back(Item("Coffee machine", 200));
	items.push_back(Item("Bastu", 500));
	items.push_back(Item("Raggarbil", 1000));

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

		// State and wish
		std::cout << "HP: " << people[i]->hp << ", Hunger: " << rsc->stomachLevel << ", Energy: " << rsc->sleepLevel << ", Money: " << rsc->money << std::endl;
		Item wish = items[people[i]->wish];
		std::cout << "Want: " << wish.name << " (" << wish.cost << ")" << std::endl;
		
		// Action
		std::string actionName = "Nothing";
		Action* act = people[i]->currentExecutiveAction;

		if (people[i]->currentExecutiveAction != nullptr)
			actionName = people[i]->currentExecutiveAction->name;

		std::cout << "Doing: " << actionName << ", At: " << people[i]->currentPlace->name << std::endl;
	}
}

void World::updateTime(double dTime, double timeScale)
{
	float prevTime = time;

	// Progress time
	lastTimeChange = dTime * timeScale;
	time += lastTimeChange;
	if (time - (int)time > 0.6f)
	{
		// Progress hour
		time = (int)time + 1 + (time - (int)time - 0.6f);

		// NPCs consider next hour schedule
		for (int i = 0; i < people.size(); i++)
		{
			people[i]->followSchedule();
		}
	}

	// Progress days
	if (time > 24)
	{
		time = time - 24.0f;
		day++;
	}

	// NPCs update
	for (int i = 0; i < people.size(); i++)
	{
		people[i]->update(dTime);
	}
}