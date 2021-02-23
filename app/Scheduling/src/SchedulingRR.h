#ifndef SCHEDULINGRR_H
#define SCHEDULINGRR_H
#include "Scheduling.h"

class SchedulingRR : public Scheduling
{
public:
	SchedulingRR() {}
	~SchedulingRR() {}
public:
	virtual QString name() const;
	virtual int     exec(const QVector<Process*>& processList, TimeMarkManager& marks);
};

#endif // SCHEDULINGRR_H
