#ifndef SCHEDULINGSRT_H
#define SCHEDULINGSRT_H
#include "Scheduling.h"

class SchedulingSRT : public Scheduling
{
public:
	SchedulingSRT() {}
	~SchedulingSRT() {}
public:
	virtual QString name() const;
	virtual int     exec(const QVector<Process*>& processList, TimeMarkManager& marks);
};

#endif // SCHEDULINGSRT_H
