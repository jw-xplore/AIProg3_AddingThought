#include "AllActions.h"

void GainMoneyAction::execute(NPCPerson* person, World* world)
{
	person->resources.money += moneyGain;
}

void BuyWishItemAction::execute(NPCPerson* person, World* world)
{
	int wish = person->wish;
	person->resources.money -= world->items[wish].cost;
	person->randomWish();
}

void EndMeetingAction::execute(NPCPerson* person, World* world)
{
	person->resources.money -= 5;
	person->meeting = nullptr;
}

void EatAction::execute(NPCPerson* person, World* world)
{
	person->currentPlace = world->diners[0];

	// Decrease energy
	person->resources.sleepLevel -= 0.002f;
	if (person->resources.sleepLevel < 0)
		person->resources.sleepLevel = 0;

	// Add food 
	if (person->resources.stomachLevel + foodAdd > 1.0f)
	{
		person->resources.stomachLevel = 1;
		return;
	}

	person->resources.stomachLevel += foodAdd;
}

void WorkAction::execute(NPCPerson* person, World* world)
{
	person->currentPlace = person->workplace;

	// Decrease food 
	person->resources.stomachLevel -= 0.01f;
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
	person->currentPlace = person->home;

	// Decrease food 
	person->resources.stomachLevel -= 0.005f;
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

void HangoutAction::execute(NPCPerson* person, World* world)
{
	// Decrease food 
	person->resources.stomachLevel -= 0.01f;
	if (person->resources.stomachLevel < 0)
		person->resources.stomachLevel = 0;

	// Decrease energy
	person->resources.sleepLevel -= 0.002f;
	if (person->resources.sleepLevel < 0)
		person->resources.sleepLevel = 0;
}

void ShopAction::execute(NPCPerson* person, World* world)
{
	person->currentPlace = world->shops[0];

	// Decrease food 
	person->resources.stomachLevel -= 0.01f;
	if (person->resources.stomachLevel < 0)
		person->resources.stomachLevel = 0;

	// Decrease energy
	person->resources.sleepLevel -= 0.01f;
	if (person->resources.sleepLevel < 0)
		person->resources.sleepLevel = 0;
}