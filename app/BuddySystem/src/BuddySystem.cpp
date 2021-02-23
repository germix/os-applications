#include "BuddySystem.h"

#include "BuddyNode.h"

#ifndef NULL
#define NULL 0
#endif

#include <QDebug>

#include <math.h>
#include <time.h>

BuddySystem::BuddySystem(int mem)
{
	root = new BuddyNode(mem);
}
BuddySystem::~BuddySystem()
{
	delete root;
}
void BuddySystem::init(int mem)
{
	delete root;
	root = new BuddyNode(mem);

	memTotal = mem;
	memUsed = 0;
	memAllocated = 0;
}
bool BuddySystem::free(QString name)
{
	if(!name.isEmpty())
	{
		BuddyNode* node = find(root, name);
		if(node != NULL)
		{
			memUsed -= node->size;
			memAllocated -= node->allocated;
			// ...
			while(node->parent != NULL)
			{
				if(node->parent->left == node)
				{
					if(!node->parent->right->isPartiallyUsed())
						node = node->parent;
					else
						break;
				}
				else if(node->parent->right == node)
				{
					if(!node->parent->left->isPartiallyUsed())
						node = node->parent;
					else
						break;
				}
			}
			node->name = "";
			node->used = false;
			if(node->isSplitted())
			{
				delete node->left;
				delete node->right;

				node->left = NULL;
				node->right = NULL;
			}
			return true;
		}
	}
	return false;
}

BuddyNode* BuddySystem::alloc(int size, QString name)
{
	if(name.isEmpty())
	{
		static int s = 123456789;
		do
		{
			s += time(0);
			qsrand(s);

			name = "ptr" + QString::number(qrand());
		}
		while(NULL != find(root, name));
	}
	if(NULL != find(root, name))
	{
		int idx = 1;
		while(NULL != find(root, name+QString::number(idx)))
		{
			idx++;
		}
		name += QString::number(idx);
	}
	BuddyNode* node = alloc(root, size, name);
	if(node != NULL)
	{
		node->allocated = size;
		// ...
		memUsed += node->size;
		memAllocated += node->allocated;
	}
	return node;
}
BuddyNode* BuddySystem::find(BuddyNode* node, const QString& name)
{
	if(node == NULL)
		return NULL;
	if(node->name == name)
		return node;

	BuddyNode* found;
	if(NULL == (found = find(node->left, name)))
	{
		found = find(node->right, name);
	}
	return found;
}
BuddyNode* BuddySystem::alloc(BuddyNode* node, int size, const QString& name)
{
	if(size <= node->size)
	{
		int div = (node->size/2);
		BuddyNode* subNode;

		if(size <= div)
		{
			if(!node->used)
			{
				if(node->left == NULL) node->left = new BuddyNode(div, node);
				if(node->right == NULL) node->right = new BuddyNode(div, node);

				if(NULL != (subNode = alloc(node->left, size, name))) return subNode;
				if(NULL != (subNode = alloc(node->right, size, name))) return subNode;
			}
		}
		if(!node->used && !node->left)
		{
			node->used = true;
			node->name = name;
			return node;
		}
	}
	return NULL;
}
