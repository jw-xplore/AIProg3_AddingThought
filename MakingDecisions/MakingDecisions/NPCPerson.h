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
	double startTime;
	double endTime;
	NPCAction action;
	int cost;
};

class NPCPerson
{
public:
	float hp;
	float stomachLevel;
	float socialLevel;
	float sleepLevel;

	int money;

	// desire - stuff that person wants to buy
	// address - where person lives/sleep
	// workplace - where person works

	NPCAction currentAction;
	double actionTime; // How long will action take
	double actionTimeSpent; // How long is person doing the action

	NPCPerson();
	void performAction(NPCAction action);

	void work();
	void eat();
	void sleep();
	void hangout();
	void shop();
};

