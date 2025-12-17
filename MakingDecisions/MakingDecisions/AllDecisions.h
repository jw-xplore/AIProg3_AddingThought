#pragma once
#include "DecisionTree.h"

class IsHungryDecision : public Decision
{
public:
	float minLevel = 0.2f;

	IsHungryDecision(float level) { minLevel = level; }
	virtual bool pass(NPCPerson* person, World* world) override;
};

class HasEnergyDecision : public Decision
{
public:
	float minLevel = 0.2f;

	HasEnergyDecision(float level) { minLevel = level; }
	virtual bool pass(NPCPerson* person, World* world) override;
};

class HasMoneyDecision : public Decision
{
public:
	int minLevel = 2;

	HasMoneyDecision(int level) { minLevel = level; }
	virtual bool pass(NPCPerson* person, World* world) override;
};
