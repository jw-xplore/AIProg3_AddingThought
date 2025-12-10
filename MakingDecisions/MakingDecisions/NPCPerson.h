#pragma once
#include <vector>
#include <string>
#include "Buildings.h"

class World;

enum NPCAction
{
	None,
	Work,
	Eat,
	SleepHome,
	Hangout,
	Shop,
};

struct NPCResources
{
	float stomachLevel;		// Hunger
	float sleepLevel;
	int money;
};

struct TimedScheduleEntry
{
	int start;
	int end;
	NPCAction action;

	TimedScheduleEntry(int st, int en, NPCAction type)
	{
		start = st;
		end = en;
		action = type;
	}
};

struct ScheduleEntry
{
	NPCAction action;
	Building* place;

	ScheduleEntry()
	{
		action = NPCAction::None;
		place = nullptr;
	}

	ScheduleEntry(NPCAction act, Building* plc)
	{
		action = act;
		place = plc;
	}
};

class NPCPerson
{
public:
	std::string name;
	float hp;
	NPCResources resources;

	World* world;
	// desire - stuff that person wants to buy
	Building* home; // where person lives/sleep
	Workplace* workplace;

	NPCAction currentAction;
	Building* currentPlace;
	double actionTime; // How long will action take
	double actionTimeSpent; // How long is person doing the action

	// Schedule
	std::vector<TimedScheduleEntry> preplannedSchedule;
	ScheduleEntry* schedule = new ScheduleEntry[24];

	NPCPerson(std::string name, World* world, int money, Building* home, Workplace* workplace);
	void performAction(NPCAction action);
	void planDay();
	std::string actionName(NPCAction action);

	void followSchedule(float time, float prevTime);

	void work();
	void eat();
	void sleepHome();
	void hangout();
	void shop();
};

