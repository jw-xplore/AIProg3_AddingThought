#pragma once

#include "DecisionTree.h"
/*
Holds reference to all decision trees definitions
*/
class TreeManager
{
public:
	TreeManager();

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
	void DefineHnagoutTree();
	void DefineShopTree();
	void DefineFreeTree();
};