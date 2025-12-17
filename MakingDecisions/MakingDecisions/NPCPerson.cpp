#include "NPCPerson.h"
#include "DecisionTree.h"
#include "World.h"
#include "TreeManager.h"
#include "Message.h"
#include <random>

NPCPerson::NPCPerson(std::string name, World* world, int money, Building* home, Workplace* workplace)
{
	this->name = name;
	this->world = world;

	this->hp = 1;
	this->resources.money = money;
	this->resources.sleepLevel = 0.9;
	this->resources.stomachLevel = 0.7;

	this->home = home;
	this->workplace = workplace;

	currentPlace = home;
	randomWish();
	treeManager = new TreeManager(this);

	// Setup stable schedule - fill with work, 8h sleep, wake up 1h before work, eat before work, at lunch time and after work
	int startW = workplace->startTime;
	int endW = workplace->endTime;

	preplannedSchedule.push_back(TimedScheduleEntry(0, startW - 1, NPCAction::SleepHome));
	preplannedSchedule.push_back(TimedScheduleEntry(startW - 1, startW, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(startW, workplace->lunchTime, NPCAction::Work));
	preplannedSchedule.push_back(TimedScheduleEntry(workplace->lunchTime, workplace->lunchTime + 1, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(workplace->lunchTime + 1, endW, NPCAction::Work));
	preplannedSchedule.push_back(TimedScheduleEntry(endW, endW + 1, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(23 - 8 + startW, 24, NPCAction::SleepHome));

	// Setup schedule
	planDay();
	followSchedule();

	// Set random time to reachout
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(2, 4);
	reachOutTime = dist(gen);

	// Start action
	int currentHour = world->time;
	currentAction = schedule[currentHour].action;
	currentPlace = schedule[currentHour].place;
}

void NPCPerson::update(float dTime)
{
	// React to messages 
	for (int i = 0; i < messagesQueue.size(); i++)
	{
		//messagesQueue[i].

		Message* msg = new Message();
		msg->inPerson = messagesQueue[i]->inPerson;
		this->world->logMessage(*msg, this, messagesQueue[i]->sender);
	}

	messagesQueue.clear();

	// Reach out to people - send message
	reachOutTime -= dTime;
	if (reachOutTime <= 0)
	{
		// Pick random person to message
		NPCPerson* recipient = world->randomPerson(this);
		sendMessage(recipient);

		// Set random time to reachout
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(4, 10);

		reachOutTime = dist(gen);
	}

	// Execute current action
	if (currentExecutiveAction != nullptr)
		currentExecutiveAction->execute(this, this->world);

	// Decrease hp if deprived of basics
	bool deprived = false;
	if (resources.stomachLevel <= 0 || resources.sleepLevel <= 0)
		deprived = true;

	if (deprived)
	{
		hp -= 0.02f;
	}
	else if (hp < 1)
	{
		hp += 0.02f;
	}

	// Die 
	if (hp <= 0)
		delete this;
}

void NPCPerson::performAction(NPCAction action)
{
	switch (action)
	{
	case NPCAction::Eat: break;
	case NPCAction::Work: break;
	case NPCAction::SleepHome: break;
	case NPCAction::Hangout: break;
	case NPCAction::Shop: break;
	}
}

/*
Preplan day schedule to setup base activities
*/
void NPCPerson::planDay()
{
	// Clear schedule
	for (int i = 0; i < 24; i++)
	{
		schedule[i] = ScheduleEntry(NPCAction::None, home);
	}

	// Assign schedule
	for (int i = 0; i < preplannedSchedule.size(); i++)
	{
		int start = preplannedSchedule[i].start;
		int end = preplannedSchedule[i].end;
		NPCAction action = preplannedSchedule[i].action;

		// Building
		Building* place = home;
		switch (action)
		{
		case NPCAction::Eat: place = world->diners[0]; break;
		case NPCAction::Work: place = workplace; break;
		}

		// Add to each hour
		for (int x = start; x < end; x++)
		{
			schedule[x] = ScheduleEntry(action, place);
		}
	}
}

/*
Provides string format name to each action
*/
std::string NPCPerson::actionName(NPCAction action)
{
	switch (action)
	{
	case None:  return "Idle";
	case Work: return "Work";
	case Eat: return "Eat";
	case SleepHome: return "Sleep";
	case Hangout: return "Hangout";
	case Shop: return "Shop";
	default:
		break;
	}
	
	return "";
}

/*
Call closing action before moving onto another
*/
void NPCPerson::finishCurretAction()
{
	if (!currentExecutiveAction)
		return;

	if (currentExecutiveAction->finalAction)
		currentExecutiveAction->finalAction->execute(this, this->world);
}

/*
Perform current action in schedule based on time
	- Plan next actions for free slots??
*/
void NPCPerson::followSchedule()
{
	// Finish action
	finishCurretAction();

	// Switch to new action
	int time = (int)world->time;

	currentAction = schedule[time].action;
	DecisionTreeNode* foundNode = nullptr;

	switch (currentAction)
	{
	case NPCAction::None: foundNode = treeManager->freeTree->makeDecision(this, this->world); break;
	case NPCAction::Work: foundNode = treeManager->workTree->makeDecision(this, this->world); break;
	case NPCAction::Eat: foundNode = treeManager->eatTree->makeDecision(this, this->world); break;
	case NPCAction::SleepHome: foundNode = treeManager->sleepTree->makeDecision(this, this->world); break;
		//case NPCAction::Hangout: foundNode = world->treeManager->eatTree->makeDecision(this, this->world); break;
		//case NPCAction::Shop: foundNode = world->treeManager->eatTree->makeDecision(this, this->world); break;
	}

	currentExecutiveAction = dynamic_cast<Action*>(foundNode);
}

/*
Pick random id for item that NPC will want to buy
*/
void NPCPerson::randomWish()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, world->items.size() - 1);

	wish = dist(gen);
}

int NPCPerson::wishCost()
{
	return this->world->items[wish].cost;
}

/*
Sending messages to other NPCs
*/
void NPCPerson::sendMessage(NPCPerson* recipient)
{
	Message* msg = new Message();
	msg->sender = this;
	msg->inPerson = this->currentPlace == recipient->currentPlace;

	recipient->messagesQueue.push_back(msg);
	this->world->logMessage(*msg, this, recipient);
}