#include "SchedulingFIFO.h"

#include "AbstractContext.h"
#include "TimeMarkManager.h"

#include <QQueue>

#include "AbstractContext2.h"

class ContextFIFO : public AbstractContext2
{
public:
	int cur;
public:
	ContextFIFO(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext2(p, m), cur(-1)
	{
	}
	virtual void onBurstTime(int time)
	{
		if(-1 != (cur = getCurrentProgram(time, cur)))
		{
			if(checkCPU(time, cur))
			{
			}
		}
	}
	int getCurrentProgram(int time, int cur)
	{
		if(queueCpu.isEmpty())
		{
			return -1;
		}
		int idx = queueCpu.head();

		if(cur != idx)
		{
			Process* p;
			ProcessState* ps;
			ExecutionFragment* ef;

			p = progs[idx];
			ps = p->getState();
			if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
			{
				marks.addMark(idx, time, TimeMark::TYPE_WAIT);
			}
		}
		return idx;
	}

	virtual int onCurrentProgram(int time)
	{
		if(queueCpu.isEmpty())
		{
			return -1;
		}
		int idx = queueCpu.head();

		if(cur != idx)
		{
			Process* p;
			ProcessState* ps;
			ExecutionFragment* ef;

			p = progs[idx];
			ps = p->getState();
			if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
			{
				marks.addMark(idx, time, TimeMark::TYPE_WAIT);
			}
		}
		cur = idx;
		return idx;
	}
	virtual void onIoTimeFinished(int time, int index, Process* prog)
	{
		if(!prog->isFinished())
		{
			queueCpu.enqueue(index);
		}
	}
	virtual void onBurstTimeFinished(int time, int index, Process* prog)
	{
		queueCpu.dequeue();
	}
};

QString SchedulingFIFO::name() const
{
	return "FIFO";
}
int     SchedulingFIFO::exec(const QVector<Process*>& processList, TimeMarkManager& marks)
{
	return ContextFIFO(processList, marks).run();
}
