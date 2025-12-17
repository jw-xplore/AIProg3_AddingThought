#pragma once
#include "DecisionTree.h"

class NPCPerson;

/*
Holds reference to all decision trees definitions
*/
class TreeManager
{
public:
	NPCPerson* owner;

	TreeManager(NPCPerson* person);

	// Trees
	DecisionTreeNode* workTree;
	DecisionTreeNode* eatTree;
	DecisionTreeNode* sleepTree;
	DecisionTreeNode* hangoutTree;
	DecisionTreeNode* shopTree;
	DecisionTreeNode* freeTree; // Use this behavior if has free schedule

	void DefineWorkTree();
	void DefineEatTree();
	void DefineSleepTree();
	void DefineHangoutTree();
	void DefineShopTree();
	void DefineFreeTree();
};