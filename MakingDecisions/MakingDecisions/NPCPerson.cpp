#include "NPCPerson.h"
#include "World.h"

NPCPerson::NPCPerson(std::string name, World* world, int money, Building* home, Workplace* workplace)
{
	this->name = name;
	this->world = world;

	this->resources.money = money;
	this->resources.sleepLevel = 0.9;
	this->resources.stomachLevel = 0.7;

	this->home = home;
	this->workplace = workplace;

	currentPlace = home;

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

	// Start action
	int currentHour = world->time;
	currentAction = schedule[currentHour].action;
	currentPlace = schedule[currentHour].place;
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
		case NPCAction::Eat: place = world->findBuidling<Diner>(); break;
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
Perform current action in schedule based on time
	- Plan next actions for free slots??
*/
void NPCPerson::followSchedule(float time, float prevTime)
{
	if ((int)prevTime < (int)time || (int)prevTime == 23 && (int)time == 0)
	{
		// End action - receive gains and loses

		// TODO: Make system based on tasks/action and rewards

		// Start action
		currentAction = schedule[(int)time].action;
		currentPlace = schedule[(int)time].place;
	}

	//currentAction = schedule[(int)time];
}