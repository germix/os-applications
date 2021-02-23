#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <QString>
#include <QVector>

class Process;
class TimeMarkManager;

class Scheduling
{
public:
	Scheduling() {}
	virtual ~Scheduling() {}
public:
	virtual QString name() const = 0;
	virtual int     exec(const QVector<Process*>& processList, TimeMarkManager& marks) = 0;
};

#endif // SCHEDULING_H
