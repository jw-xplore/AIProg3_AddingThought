#include "AllActions.h"

void EatAction::execute(NPCPerson* person, World* world)
{
	if (person->resources.stomachLevel + foodAdd > 1.0f)
	{
		person->resources.stomachLevel = 1;
		return;
	}

	person->resources.stomachLevel += foodAdd;
}

void WorkAction::execute(NPCPerson* person, World* world)
{
	// Add money if hour has passed 
	int prevHour = (int)(world->time - world->lastTimeChange);

	if (prevHour < (int)world->time)
		person->resources.money += moneyGain;

	// Decrease food 
	person->resources.stomachLevel -= 0.02f;
	if (person->resources.stomachLevel < 0)
		person->resources.stomachLevel = 0;

	// Decrease energy
	if (person->resources.sleepLevel - energyLoss < 0.0f)
	{
		person->resources.sleepLevel = 0;
		return;
	}

	person->resources.sleepLevel -= energyLoss;
}

void SleepAction::execute(NPCPerson* person, World* world)
{
	// Decrease food 
	person->resources.stomachLevel -= 0.01f;
	if (person->resources.stomachLevel < 0)
		person->resources.stomachLevel = 0;

	// Increase energy
	if (person->resources.sleepLevel + energyGain > 1)
	{
		person->resources.sleepLevel = 1;
		return;
	}

	person->resources.sleepLevel += energyGain;
}