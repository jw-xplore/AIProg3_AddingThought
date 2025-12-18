#pragma once
#include "DecisionTree.h"

class GainMoneyAction : public Action
{
public:
	int moneyGain = 2;
	GainMoneyAction(int money)
	{
		moneyGain = money;
	}

	virtual void execute(NPCPerson* person, World* world) override;
};

class BuyWishItemAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;
};

class EndMeetingAction : public Action
{
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

	WorkAction(int pay)
	{ 
		name = "Working";
		finalAction = new GainMoneyAction(pay);
	}
};

class SleepAction : public Action
{
public:
	float energyGain = 0.03f;

	virtual void execute(NPCPerson* person, World* world) override;

	SleepAction() { name = "Sleeping"; }
};

class HangoutAction : public Action
{
public:
	virtual void execute(NPCPerson* person, World* world) override;

	HangoutAction() 
	{ 
		name = "Hanging out"; 
		finalAction = new EndMeetingAction();
	}
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
