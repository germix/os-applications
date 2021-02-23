#ifndef SCHEDULINGFIFO_H
#define SCHEDULINGFIFO_H
#include "Scheduling.h"

class SchedulingFIFO : public Scheduling
{
public:
	SchedulingFIFO() {}
	~SchedulingFIFO() {}
public:
	virtual QString name() const;
	virtual int     exec(const QVector<Process*>& processList, TimeMarkManager& marks);
};

#endif // SCHEDULINGFIFO_H
