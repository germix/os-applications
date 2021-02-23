#ifndef BUDDYSYSTEM_H
#define BUDDYSYSTEM_H
#include <QString>

class BuddyNode;

class BuddySystem
{
	BuddyNode* root;
public:
	int			memTotal;
	int			memUsed;
	int			memAllocated;
public:
	BuddySystem(int mem);
	~BuddySystem();
public:
	BuddyNode* getRoot() const { return root; }

	void init(int mem);
	bool free(QString name);
	BuddyNode* alloc(int size, QString name = "");
	BuddyNode* find(QString name)
	{
		return find(root, name);
	}
private:
	BuddyNode* find(BuddyNode* node, const QString& name);
	BuddyNode* alloc(BuddyNode* node, int size, const QString& name);
public:

};

#endif // BUDDYSYSTEM_H
