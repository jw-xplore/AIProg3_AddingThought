#include "World.h"
#include <iostream>
#include "TreeManager.h"
#include "Message.h";
#include <random>

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
	/*
	Building* home1 = new Building("House 001");
	Building* home2 = new Building("House 002");
	Workplace* factory = new Workplace("Factory", 2, 0.1, 7, 12, 15);
	Building* shop = new Building("Market world", 2, 0.1, 7, 12, 15);
	*/

	/*
	buildings.push_back(home1);
	buildings.push_back(factory);
	buildings.push_back(new Diner("Buffet", 2, 0.3, 0.05));
	*/

	Building* home1 = new Building("House 001");
	Building* home2 = new Building("House 002");
	Workplace* factory = new Workplace("Factory", 2, 0.1, 7, 12, 15);

	houses.push_back(home1);
	houses.push_back(home2);

	workplaces.push_back(factory);

	shops.push_back(new Building("Market world"));

	diners.push_back(new Diner("Buffet", 2, 0.1f, 0.01f));
	bars.push_back(new Bar("Drunk Elk", 5, 0.1));

	// People
	people.push_back(new NPCPerson("Bjorn", this, 10, home1, factory));
	people.push_back(new NPCPerson("Aron", this, 15, home2, factory));
	people.push_back(new NPCPerson("Linda", this, 15, home2, factory));
	people.push_back(new NPCPerson("Vilma", this, 15, home2, factory));

	// Setup message log
	for (int i = 0; i < messagesLogSize; i++)
	{
		messagesLog[i] = "";
	}
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

/*
Show recent logged messages between NPCs
*/
void World::showMessagesLog()
{
	for (int i = 0; i < messagesLogSize; i++)
	{
		std::cout << messagesLog[i] << std::endl;
	}
}

/*
Progress time and day and apply changes on NPCs
*/
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

/*
Save message into log
*/
void World::logMessage(Message msg, NPCPerson* sender, NPCPerson* recipient)
{
	std::string content = "(" + sender->name + "->" + recipient->name + ")" + msg.content;
	if (!msg.inPerson)
		content = "SMS " + content;

	if (lastMsg < messagesLogSize - 1)
	{
		// Progress messages
		lastMsg++;
		messagesLog[lastMsg] = content;
	}
	else
	{
		// Push messages
		for (int i = 1; i < messagesLogSize; i++)
		{
			messagesLog[i - 1] = messagesLog[i];
		}

		messagesLog[messagesLogSize - 1] = content;
	}
}

/*
* Pick any person that is not one to be excluded
*/

NPCPerson* World::randomPerson(NPCPerson* exclude)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, this->people.size() - 1);

	int id = dist(gen);

	if (people[id] == exclude)
	{
		if (id == 0)
			id++;
		else
			id--;
	}

	return people[id];
}