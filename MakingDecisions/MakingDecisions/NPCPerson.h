#pragma once

enum NPCAction
{
	Work,
	Eat,
	Sleep,
	Hangout,
	Shop,
};

struct ScheduleEntry
{
	NPCAction action;
	NPCResources gainCost;
};

struct TimedScheduleEntry
{
	float start;
	float end;
	NPCAction action;
	NPCResources gainCost;
};

struct NPCResources
{
	float stomachLevel;		// Hunger
	float hydratationLevel; // Thirst
	float socialLevel;		// Friends
	float sleepLevel;

	int money;
};

class NPCPerson
{
public:
	float hp;
	NPCResources resources;

	// desire - stuff that person wants to buy
	// address - where person lives/sleep
	// workplace - where person works

	NPCAction currentAction;
	double actionTime; // How long will action take
	double actionTimeSpent; // How long is person doing the action

	// Schedule
	ScheduleEntry* schedule = new ScheduleEntry[24];

	NPCPerson();
	void performAction(NPCAction action);

	void work();
	void eat();
	void sleep();
	void hangout();
	void shop();
};

