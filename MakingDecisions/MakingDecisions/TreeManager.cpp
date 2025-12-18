#include "TreeManager.h"
#include "AllDecisions.h"
#include "AllActions.h"
#include "NPCPerson.h"

TreeManager::TreeManager(NPCPerson* person)
{
	owner = person;

	// Define decision trees
	DefineWorkTypeTree();
	DefineFreeTree();
	DefineEatTree();

	DefineWorkTree();
	DefineSleepTree();
	DefineHangoutTree();
}

void TreeManager::DefineWorkTree()
{
	workTree = new HasEnergyDecision(0.25f);

	// Check if has enough energy
	Decision* energyDec = dynamic_cast<Decision*>(workTree);
	energyDec->positive = new IsHungryDecision(0.2f);
	energyDec->negative = new SleepAction(); // Go sleep

	// If enough energy, check hunger
	Decision* hungerDec = dynamic_cast<Decision*>(energyDec->positive);
	hungerDec->positive = eatTree; // Eat
	hungerDec->negative = workTypeTree; // Work if not hungry
}

void TreeManager::DefineEatTree()
{
	eatTree = new IsHungryDecision(0.9f);
	Diner* diner = owner->world->diners[0];

	// Check if wants to eat
	Decision* hungerDec = dynamic_cast<Decision*>(eatTree);
	hungerDec->positive = new HasEnergyDecision(0.1f);
	hungerDec->negative = freeTree; // Consider idle action

	// If hungry, check sleepiness
	Decision* energyDec = dynamic_cast<Decision*>(hungerDec->positive);
	energyDec->positive = new HasMoneyDecision(diner->cost);
	energyDec->negative = new SleepAction(); // Go sleep

	// Check if has money
	Decision* moneyDec = dynamic_cast<Decision*>(energyDec->positive);
	moneyDec->positive = new EatAction(diner); // Go for meal
	moneyDec->negative = workTypeTree; // Make money

}

void TreeManager::DefineSleepTree()
{
	sleepTree = new HasEnergyDecision(0.99f);
	
	// Check if want to sleep
	Decision* energyDec = dynamic_cast<Decision*>(sleepTree);
	energyDec->positive = freeTree; // Consider free action
	energyDec->negative = new SleepAction(); // Go sleep
}

void TreeManager::DefineHangoutTree()
{
	hangoutTree = new IsHungryDecision(0.4f);
	Bar* bar = owner->world->bars[0];

	// Is NPC hungry?
	Decision* hungerDec = dynamic_cast<Decision*>(hangoutTree);
	hungerDec->positive = eatTree; // Eat
	hungerDec->negative = new HasEnergyDecision(0.3f);

	// Needs sleep?
	Decision* energyDec = dynamic_cast<Decision*>(hungerDec->negative);
	energyDec->positive = new HasMoneyDecision(bar->cost + 5);
	energyDec->negative = new SleepAction(); // Go sleep

	// Has enough money?
	Decision* moneyDec = dynamic_cast<Decision*>(energyDec->positive);
	moneyDec->positive = new HangoutAction(bar); // Go hangout
	moneyDec->negative = workTypeTree; // Work some more
}

/*
What to do if there is no action scheduled
*/
void TreeManager::DefineFreeTree()
{
	freeTree = new IsHungryDecision(0.6f);

	// Is NPC at least little hungry?
	Decision* hungerDec = dynamic_cast<Decision*>(freeTree);
	hungerDec->positive = new EatAction(owner->world->diners[0]); // Eat
	hungerDec->negative = new HasEnergyDecision(0.4f);

	// Needs sleep?
	Decision* energyDec = dynamic_cast<Decision*>(hungerDec->negative);
	energyDec->positive = new HasMoneyForWishDecision();
	energyDec->negative = new SleepAction(); // Go sleep

	// Can afford to buy wished item?
	Decision* shopDec = dynamic_cast<Decision*>(energyDec->positive);
	shopDec->positive = new ShopAction(); // Buy item
	shopDec->negative = workTypeTree; // Work some more
}

/*
Decides if goes working to regular workplace or to side hustle factory
*/
void TreeManager::DefineWorkTypeTree()
{
	workTypeTree = new WorkplaceIsOpenDecision();

	// Is workplace open
	Decision* workplaceOpenDec = dynamic_cast<Decision*>(workTypeTree);
	workplaceOpenDec->positive = new WorkAction(this->owner->workplace, false); // Regular work
	workplaceOpenDec->negative = new WorkAction(this->owner->world->sidehustleFactory, true); // Side hustle
}