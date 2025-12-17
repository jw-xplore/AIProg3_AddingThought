#pragma once
#include "Buildings.h"
#include "NPCPerson.h"
#include <vector>
#include <type_traits>
#include <string>

class TreeManager;
class Message;

/*
World representing buldings and players
*/
class World
{
public:
	std::vector<NPCPerson*> people;
	std::vector<Item> items;

	// Buildings
	std::vector<Building*> houses;
	std::vector<Building*> shops;
	std::vector<Diner*> diners;
	std::vector<Bar*> bars;
	std::vector<Workplace*> workplaces;

	int messagesLogSize = 10;
	int lastMsg = -1;
	std::string* messagesLog = new std::string[messagesLogSize];


	double lastTimeChange = 0;
	double time = 15.0f;
	int day = 1;

	World();

	void showPeopleStatus();
	void showMessagesLog();
	void updateTime(double dTime, double timeScale);

	void logMessage(Message msg, NPCPerson* sender, NPCPerson* recipient);

	NPCPerson* randomPerson(NPCPerson* exclude);
};

