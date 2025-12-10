#pragma once
#include <string>

class Building
{
public:
	std::string name;
	
	Building() {}

	Building(std::string name)
	{
		this->name = name;
	}

	virtual ~Building()
	{

	}
};

class Workplace : public Building
{
public:
	// Hourly salaray and energy lost
	int salary;
	float energyLoss;

	// In which hour
	int startTime;
	int lunchTime;
	int endTime;


	Workplace(std::string name, int money, float energy, int start, int lunch, int end)
	{
		this->name = name;
		salary = money;
		energyLoss = energy;

		startTime = start;
		lunchTime = lunch;
		endTime = end;
	}
};

class Diner : public Building
{
public:
	int cost;
	float stomachLevelGain;
	float energyGain;

	Diner(std::string name, int money, float food, float energy)
	{
		this->name = name;
		cost = money;
		stomachLevelGain = food;
		energyGain = energy;
	}
};

class Bar : public Building
{
public:
	int cost;
	float energyLoss;

	Bar(std::string name, int money, float energy)
	{
		this->name = name;
		cost = money;
		energyLoss = energy;
	}
};