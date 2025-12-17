#pragma once
#include "DecisionTree.h"

class GainMoneyAction : public Action
{
public:
	int moneyGain = 2;
	virtual void execute(NPCPerson* person, World* world) override;
};

class BuyWishItemAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;
};

class EatAction : public Action
{
public:
	float foodAdd = 0.1f;

	virtual void execute(NPCPerson* person, World* world) override;

	EatAction() { name = "Eating"; }
};

class WorkAction : public Action
{
public:
	float energyLoss = 0.01f;
	int moneyGain = 2; // Hourly 

	virtual void execute(NPCPerson* person, World* world) override;

	WorkAction()
	{ 
		name = "Working";
		finalAction = new GainMoneyAction();
	}
};

class SleepAction : public Action
{
public:
	float energyGain = 0.05f;

	virtual void execute(NPCPerson* person, World* world) override;

	SleepAction() { name = "Sleeping"; }
};

class ShopAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;

	ShopAction()
	{ 
		name = "Shopping"; 
		finalAction = new BuyWishItemAction();
	}
};


/*
class HangoutAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;

	HangoutAction() { name = "Hanging out"; }
};

class ShopAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;

	ShopAction() { name = "Shopping"; }
};
*/
