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