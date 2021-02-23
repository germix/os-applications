#include "BuddyNode.h"

BuddyNode::BuddyNode(int size__, BuddyNode* parent__)
	:
	size(size__),
	parent(parent__),
	left(0),
	right(0),
	used(false)
{
	x = 0;
	y = 0;
	width = 0;
	allocated = 0;
	highlight = false;
}
BuddyNode::~BuddyNode()
{
	if(left) delete left;
	if(right) delete right;
}
bool       BuddyNode::isSplitted() const
{
	return (left || right);
}

bool		BuddyNode::isPartiallyUsed() const
{
	if(used)
		return true;
	if(left != NULL && left->isPartiallyUsed())
		return true;
	if(right != NULL && right->isPartiallyUsed())
		return true;
	return false;
}
