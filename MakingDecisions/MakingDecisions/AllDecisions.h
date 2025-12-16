#pragma once
#include "DecisionTree.h"

class IsHungryDecision : public Decision
{
public:
	float minLevel = 0.2f;

	virtual bool pass(NPCPerson* person, World* world) override;
};

