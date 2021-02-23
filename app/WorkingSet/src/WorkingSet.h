#ifndef WORKINGSET_H
#define WORKINGSET_H
#include <QVector>

class WorkingSetListener
{
public:
	WorkingSetListener() {}
	virtual ~WorkingSetListener() {}
public:
	virtual void init() = 0;
	virtual void step(int num, const QVector<int>& wset) = 0;
};

class WorkingSet
{
public:
    WorkingSet();
	~WorkingSet();
public:
	int run(WorkingSetListener* listener, int wsize, QVector<int> sequence);
};

#endif // WORKINGSET_H
