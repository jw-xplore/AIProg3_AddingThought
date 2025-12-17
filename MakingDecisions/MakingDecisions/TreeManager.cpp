#include "TreeManager.h"
#include "AllDecisions.h"
#include "AllActions.h"

TreeManager::TreeManager()
{
	/*--------------------------------------------------------------
	Define work tree
	--------------------------------------------------------------*/
	DefineWorkTree();
	DefineEatTree();

	// Define eat tree

	// Define sleep tree

	// Define hangout tree

	// Define shop tree

	// Define  tree
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
	hungerDec->positive = new HasMoneyDecision(2);
	hungerDec->negative = new WorkAction(); // Work if not hungry

	// If hungry, check if can eat
	Decision* moneyDec = dynamic_cast<Decision*>(hungerDec->positive);
	moneyDec->positive = new EatAction(); // Go for meal
	//std::string str = dynamic_cast<Action*>(moneyDec->positive)->name;
	moneyDec->negative = new WorkAction(); // Make money
}

void TreeManager::DefineEatTree()
{
	eatTree = new IsHungryDecision(0.9f);

	// Check if wants to eat
	Decision* hungerDec = dynamic_cast<Decision*>(eatTree);
	hungerDec->positive = new HasEnergyDecision(0.1f);
	hungerDec->negative = freeTree; // Consider idle action

	// If hungry, check sleepiness
	Decision* energyDec = dynamic_cast<Decision*>(hungerDec->positive);
	energyDec->positive = new HasMoneyDecision(2);
	energyDec->negative = new SleepAction(); // Go sleep

	// Check if has money
	Decision* moneyDec = dynamic_cast<Decision*>(hungerDec->positive);
	moneyDec->positive = new EatAction(); // Go for meal
	moneyDec->negative = new WorkAction(); // Make money

}

void TreeManager::DefineSleepTree()
{
	sleepTree = new HasEnergyDecision(0.9f);
	
	// Check if want to sleep
	Decision* energyDec = dynamic_cast<Decision*>(sleepTree);
	energyDec->positive = freeTree; // Consider free action
	energyDec->negative = new SleepAction(); // Go sleep
}