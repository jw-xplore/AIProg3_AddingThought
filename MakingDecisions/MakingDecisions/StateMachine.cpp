#include "StateMachine.h"

StateMachine::StateMachine(State* initState)
{
	currentState = initState;
}

std::vector<Action*> StateMachine::update()
{
	Transition* currentTrans = nullptr;

	// Get first triggered transition
	for (int i = 0; i < currentState->transitions.size(); i++)
	{
		if (currentState->transitions[i]->triggered)
		{
			currentTrans = currentState->transitions[i];
			break;
		}
	}

	// Just get current action
	if (currentTrans == nullptr)
		return { currentState->action };

	// Move to another state
	currentState = currentTrans->targetState;
	return { currentState->action };
}