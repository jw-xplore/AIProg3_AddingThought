#include "NPCPerson.h"
#include "DecisionTree.h"
#include "World.h"
#include "TreeManager.h"
#include "Message.h"
#include <random>
#include "AllActions.h"

NPCPerson::NPCPerson(std::string name, World* world, int money, Building* home, Workplace* workplace)
{
	this->name = name;
	this->world = world;

	this->hp = 1;
	this->resources.money = randomNum(10, 50);;
	this->resources.sleepLevel = 0.9;
	this->resources.stomachLevel = 0.8;

	this->home = home;
	this->workplace = workplace;

	currentPlace = home;
	randomWish();
	treeManager = new TreeManager(this);

	// Setup stable schedule - fill with work, 8h sleep, wake up 1h before work, eat before work, at lunch time and after work
	int startW = workplace->startTime;
	int endW = workplace->endTime;
	sleepHour = 23 - 8 + startW;

	preplannedSchedule.push_back(TimedScheduleEntry(0, startW - 1, NPCAction::SleepHome));
	preplannedSchedule.push_back(TimedScheduleEntry(startW - 1, startW, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(startW, workplace->lunchTime, NPCAction::Work));
	preplannedSchedule.push_back(TimedScheduleEntry(workplace->lunchTime, workplace->lunchTime + 1, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(workplace->lunchTime + 1, endW, NPCAction::Work));
	preplannedSchedule.push_back(TimedScheduleEntry(endW, endW + 1, NPCAction::Eat));
	preplannedSchedule.push_back(TimedScheduleEntry(sleepHour, 24, NPCAction::SleepHome));

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
	handleMessageQueue();

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

	// Readd meeting from yesterday
	if (meeting)
	{
		schedule[meeting->hour] = ScheduleEntry(NPCAction::Hangout, meeting->place);
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
	case NPCAction::Hangout: foundNode = treeManager->hangoutTree->makeDecision(this, this->world); break;
		//case NPCAction::Shop: foundNode = world->treeManager->eatTree->makeDecision(this, this->world); break;
	}

	// Check if meeting is not happening?
	if (currentAction == NPCAction::Hangout && dynamic_cast<HangoutAction*>(foundNode) == nullptr && meeting)
	{
		// Log cancelation
		Message msg = Message();
		msg.sender = this;
		msg.inPerson = false;
		msg.content = "Sorry, I got to cancel.";

		// Reason
		if (dynamic_cast<EatAction*>(foundNode))
			msg.content += " I am starving.";
		else if (dynamic_cast<SleepAction*>(foundNode))
			msg.content += " I am tired.";
		else if (dynamic_cast<WorkAction*>(foundNode))
			msg.content += " I am out of money.";

		this->world->logMessage(msg, this, meeting->sender);

		// Cancel meeting
		meeting->sender->meeting = nullptr;
		meeting = nullptr;
	}

	// Report hanging out
	if (currentAction == NPCAction::Hangout && dynamic_cast<HangoutAction*>(foundNode) && meeting)
	{
		Message msg = Message();
		msg.sender = this;
		msg.inPerson = true;
		msg.content = "Skal! *clink*clink";

		this->world->logMessage(msg, this, meeting->sender);
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
	// Skip if sleeping 
	if (currentAction == NPCAction::SleepHome)
		return;

	// Try send invite
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 3);
	if (dist(gen) > 2)
	{
		sendInvite(recipient);
		return;
	}

	// Send simple message 
	Message* msg = new Message();
	msg->sender = this;
	msg->inPerson = this->currentPlace == recipient->currentPlace;

	// Send out
	recipient->messagesQueue.push_back(msg);
	this->world->logMessage(*msg, this, recipient);
}

void NPCPerson::sendInvite(NPCPerson* recipient)
{
	// Skip if already has meeting
	if (meeting != nullptr)
		return;

	// Message setup
	InviteMessage* msg = new InviteMessage();
	msg->sender = this;
	msg->inPerson = this->currentPlace == recipient->currentPlace;
	msg->place = this->world->bars[0];

	// Hour
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(this->workplace->endTime + 1, sleepHour - 1);

	msg->hour = dist(gen);

	msg->content = "Wanna meet in " + msg->place->name + " at " + std::to_string(msg->hour) + "?";

	// Send out
	recipient->messagesQueue.push_back(msg);
	this->world->logMessage(*msg, this, recipient);
}

/*
React to all incoming messages
*/
void NPCPerson::handleMessageQueue()
{
	for (int i = 0; i < messagesQueue.size(); i++)
	{
		// Invite response
		InviteMessage* invite = dynamic_cast<InviteMessage*>(messagesQueue[i]);
		if (invite != nullptr)
		{
			bool positive = true;

			if (schedule[invite->hour].action != NPCAction::None || meeting != nullptr)
			{
				// Say no if blocked
				positive = false;
			}
			else
			{
				// Randomly say yes or no
				static std::random_device rd;
				static std::mt19937 gen(rd());
				std::uniform_int_distribution<> dist(0, 2);

				if (dist(gen) == 0)
					positive = false;
			}

			invite->respond(positive);

			// Send out
			Message* msg = new Message();
			msg->inPerson = messagesQueue[i]->inPerson;

			if (positive)
			{
				msg->content = "Yes!";

				// Store in schedule
				meeting = invite;
				this->schedule[invite->hour].action = NPCAction::Hangout;
				this->schedule[invite->hour].place = invite->place;
			}
			else
				msg->content = "No, can't make it.";

			this->world->logMessage(*msg, this, messagesQueue[i]->sender);

			continue;
		}

		// Response to basic
		Message* msg = new Message();
		msg->inPerson = messagesQueue[i]->inPerson;
		this->world->logMessage(*msg, this, messagesQueue[i]->sender);
	}

	// Remove all
	messagesQueue.clear();
}

float NPCPerson::randomNum(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}