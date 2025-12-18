#include "AllDecisions.h"
#include "NPCPerson.h"
#include "World.h"

bool IsHungryDecision::pass(NPCPerson* person, World* world)
{
	return person->resources.stomachLevel < minLevel;
}

bool HasEnergyDecision::pass(NPCPerson* person, World* world)
{
	return person->resources.sleepLevel > minLevel;
}

bool HasMoneyDecision::pass(NPCPerson* person, World* world)
{
	return person->resources.money > minLevel;
}

bool HasMoneyForWishDecision::pass(NPCPerson* person, World* world)
{
	return person->resources.money + 20 > person->wishCost();
}

/*
Return true if current time is between start and end of persons workplace shift
*/
bool WorkplaceIsOpenDecision::pass(NPCPerson* person, World* world)
{
	int s = person->workplace->startTime;
	int e = person->workplace->endTime;

	return s <= world->time && world->time <= e - 1;
}