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
};