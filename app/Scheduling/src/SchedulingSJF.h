#ifndef SCHEDULINGSJF_H
#define SCHEDULINGSJF_H
#include "Scheduling.h"

class SchedulingSJF : public Scheduling
{
public:
	SchedulingSJF() {}
	~SchedulingSJF() {}
public:
	virtual QString name() const;
	virtual int     exec(const QVector<Process*>& processList, TimeMarkManager& marks);
};

#endif // SCHEDULINGSJF_H
