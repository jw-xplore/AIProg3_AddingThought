#pragma once
#include <string>
#include "World.h"
#include "NPCPerson.h"

class DecisionTreeNode
{
public:
	DecisionTreeNode() {}
	virtual DecisionTreeNode* makeDecision(NPCPerson* person, World* world) { return this;  }
};

class Action : public DecisionTreeNode
{
public:
	std::string name = "";
	Action* finalAction;

	virtual void execute(NPCPerson* person, World* world) {}

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
		//elete positive->makeDecision();
		//delete negative->makeDecision();
	}

	virtual bool pass(NPCPerson* person, World* world) { return true; }

	DecisionTreeNode* makeDecision(NPCPerson* person, World* world) override
	{
		if (pass(person, world))
			return positive->makeDecision(person, world);

		return negative->makeDecision(person, world);
	}
};