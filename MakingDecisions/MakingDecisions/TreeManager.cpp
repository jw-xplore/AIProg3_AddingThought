#include "TreeManager.h"
#include "AllDecisions.h"
#include "AllActions.h"

TreeManager::TreeManager()
{
	// Define work tree
	workTree = new IsHungryDecision();
	Decision* workDec = dynamic_cast<Decision*>(workTree);
	workDec->positive = new EatAction(); // Eat if hungry
	workDec->negative = new WorkAction(); // Work if not hungry

	// Define eat tree
}