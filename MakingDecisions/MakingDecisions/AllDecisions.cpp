#include "AllDecisions.h"
#include "NPCPerson.h"
#include "World.h"

bool IsHungryDecision::pass(NPCPerson* person, World* world)
{
	return person->resources.stomachLevel < minLevel;
}