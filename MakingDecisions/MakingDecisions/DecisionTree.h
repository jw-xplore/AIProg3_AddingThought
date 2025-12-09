#pragma once
#include "World.h"
#include "NPCPerson.h"

class DecisionTreeNode
{
public:
	virtual DecisionTreeNode* makeDecision(NPCPerson* person, World* world);
};

class Action : public DecisionTreeNode
{
public:
	virtual void action();

	DecisionTreeNode* makeDecision(NPCPerson* person, World* world) override
	{
		return this;
	}
};

class Decision : public DecisionTreeNode
{
public:
	DecisionTreeNode* positive;
	DecisionTreeNode* negative;

	~Decision()
	{
		delete positive->makeDecision();
		delete negative->makeDecision();
	}

	virtual bool pass(NPCPerson* person, World* world) { return true; }

	DecisionTreeNode* makeDecision(NPCPerson* person, World* world) override
	{
		if (pass(person, world))
			return positive->makeDecision(person, world);

		return negative->makeDecision(person, world);
	}
};