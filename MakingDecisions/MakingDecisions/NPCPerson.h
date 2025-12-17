#pragma once
#include <vector>
#include <string>
#include "Buildings.h"

class World;
class Action;
class TreeManager;

enum NPCAction
{
	None,
	Work,
	Eat,
	SleepHome,
	Hangout,
	Shop,
};

struct Item
{
	std::string name;
	int cost;

	Item(std::string nam, int cst) { name = nam; cost = cst; }
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

class Message;

class NPCPerson
{
public:
	std::string name;
	float hp;
	NPCResources resources;

	std::vector<Message*> messagesQueue;
	float reachOutTime = 0;

	World* world;
	int wish = -1; // stuff that person wants to buy
	Building* home; // where person lives/sleep
	Workplace* workplace;
	int sleepHour = 0;

	TreeManager* treeManager;
	NPCAction currentAction;
	Action* currentExecutiveAction;
	Building* currentPlace;
	double actionTime; // How long will action take
	double actionTimeSpent; // How long is person doing the action

	// Schedule
	std::vector<TimedScheduleEntry> preplannedSchedule;
	ScheduleEntry* schedule = new ScheduleEntry[24];

	NPCPerson(std::string name, World* world, int money, Building* home, Workplace* workplace);
	void update(float dTime);
	void performAction(NPCAction action);
	void planDay();
	std::string actionName(NPCAction action);

	void finishCurretAction();
	void followSchedule();

	void randomWish();
	int wishCost();

	void sendMessage(NPCPerson* recipient);
	void sendInvite(NPCPerson* recipient);
	void handleMessageQueue();
};

