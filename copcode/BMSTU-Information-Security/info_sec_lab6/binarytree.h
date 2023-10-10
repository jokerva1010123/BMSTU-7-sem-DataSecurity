#pragma once

#include <iostream>
#include <inttypes.h>
#include <vector>
#include <memory>

#include "frequency_table.h"

using namespace std;

struct BinaryTreeNode
{
	FreqPair mValue;
	std::shared_ptr<BinaryTreeNode> mParent;
	std::shared_ptr<BinaryTreeNode> mLeft;
	std::shared_ptr<BinaryTreeNode> mRight;
};

struct BinaryTree
{
	std::shared_ptr<BinaryTreeNode> mHead;
	vector<std::shared_ptr<BinaryTreeNode>> mLists;
};

