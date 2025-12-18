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
public:
	int cost = 0;

	virtual void execute(NPCPerson* person, World* world) override;

	EndMeetingAction(int cost)
	{
		this->cost = cost;
	}
};

class EatAction : public Action
{
public:
	Diner* diner;
	float foodAdd = 0.1f;
	int cost;

	virtual void execute(NPCPerson* person, World* world) override;

	EatAction(Diner* diner)
	{ 
		name = "Eating";

		this->diner = diner;
		foodAdd = diner->stomachLevelGain;
		cost = diner->cost;

		finalAction = new GainMoneyAction(-cost);
	}
};

class WorkAction : public Action
{
public:
	Workplace* workplace;
	float energyLoss = 0.01f;
	int moneyGain = 2; // Hourly 

	virtual void execute(NPCPerson* person, World* world) override;

	WorkAction(Workplace* workplace, bool sideHustle = false)
	{ 
		name = "Working";
		if (sideHustle)
			name += " on side";

		this->workplace = workplace;
		finalAction = new GainMoneyAction(workplace->salary);
		energyLoss = workplace->energyLoss;
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
	Bar* bar;

	virtual void execute(NPCPerson* person, World* world) override;

	HangoutAction(Bar* bar) 
	{ 
		name = "Hanging out"; 
		finalAction = new EndMeetingAction(bar->cost);
		this->bar = bar;
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
