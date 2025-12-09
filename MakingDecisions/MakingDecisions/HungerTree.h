#pragma once
#include "DecisionTree.h"

class DecIsHungry : public Decision
{
public:
	bool pass(NPCPerson* person, World* world) override
	{
		return person->resources.stomachLevel < 0.5f;
	}
};

/*
Hunger tree structure
*/
struct HungerTree
{
	Decision* root;

	HungerTree()
	{
		root = new DecIsHungry();

	}
	~HungerTree()
	{
		delete root;
	}
};