#ifndef BUDDYNODE_H
#define BUDDYNODE_H
#include <QString>

class BuddyNode
{
public:
	int				x;
	int				y;
	int				width;
private:
	int				size;
	bool			used;
	BuddyNode*		left;
	BuddyNode*		right;
	BuddyNode*		parent;


	QString			name;

	int				allocated;

	friend class BuddySystem;
public:
	bool			highlight;
public:
	BuddyNode(int size, BuddyNode* parent = 0);
	~BuddyNode();
public:
	bool       isUsed() const { return used; }
	int        getSize() const { return size; }
	BuddyNode* getLeft() const { return left; }
	BuddyNode* getRight() const { return right; }
	BuddyNode* getParent() const { return parent; }

	bool       isSplitted() const;

	QString		getName() const { return name; }

	bool		isPartiallyUsed() const;
};

#endif // BUDDYNODE_H
