#ifndef ABSTRACTCONTEXT_H
#define ABSTRACTCONTEXT_H
#include <QVector>

#include "Process.h"

class TimeMarkManager;

class AbstractContext
{
public:
	QVector<Process*>	progs;
	bool				endFlag;

	QVector<Process*>	allProgs;
protected:

	TimeMarkManager&	marks;
public:
	AbstractContext(const QVector<Process*>& p, TimeMarkManager& m);
	~AbstractContext();
public:
	bool atEnd() const
	{
		return endFlag;
	}
	virtual void checkIO(int time);
	virtual void checkArrivals(int time);



	virtual int run()
	{
		int time;
		int cur = -1;

		for(time = 0; !atEnd(); time++)
		{
			checkIO(time);
			checkArrivals(time);

			onBurstTime(time);
		}
		qDebug("time=%d", time);

		return time-1;
	}
protected:
	virtual void onBurstTime(int time)
	{
	}
	virtual void onArrivedProgram(int time, int index, Process* prog)
	{
	}
};

#endif // ABSTRACTCONTEXT_H
