#pragma once
#include "DecisionTree.h"


class Transition;

class State
{
public:
	Action* action;
	std::vector<Transition*> transitions;
};

class Transition
{
public:
	bool triggered;
	State* targetState;
};

class StateMachine
{
public:
	State* currentState;

	StateMachine(State* initState);
	std::vector<Action*> update();
};

