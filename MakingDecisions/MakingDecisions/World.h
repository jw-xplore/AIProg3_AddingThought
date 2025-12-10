#pragma once
#include "Buildings.h"
#include "NPCPerson.h"
#include <vector>
#include <type_traits>

/*
World representing buldings and players
*/
class World
{
public:
	std::vector<Building*> buildings;
	std::vector<NPCPerson*> people;

	double time = 6.0f;
	int day = 1;

	World();

	void showPeopleStatus();
	void updateTime(double dTime, double timeScale);

	/*
	Return first building of selected subtype
	*/
	template<class T>
	Building* findBuidling()
	{
		for (int i = 0; i < buildings.size(); i++)
		{
			if (dynamic_cast<const T*>(buildings[i]) != nullptr)
				return buildings[i];
		}

		return nullptr;
	}
};

