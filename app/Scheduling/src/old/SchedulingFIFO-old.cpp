#include "SchedulingFIFO.h"

#include "AbstractContext.h"
#include "TimeMarkManager.h"

class ContextFIFO : public AbstractContext
{
public:
	int curProg;
	int lastProg;
public:
	ContextFIFO(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext(p, m)
	{
		curProg = 0;
		lastProg = 0;
	}
	int getCurrentProgram(int time, int cur)
	{
		if(cur == -1)
		{
			int i = lastProg;
			Process* p;
			ProcessState* ps;
			ExecutionFragment* ef;

			while(lastProg != (i = getCyclicNext(i)))
			{
				p = progs[i];
				ps = p->getState();
				if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
				{
					marks.addMark(i, time, TimeMark::TYPE_WAIT);
					return i;
				}
			}
			if(i == lastProg)
			{
				p = progs[i];
				ps = p->getState();
				if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
				{
					return i;
				}
			}
			return -1;
		}
		return cur;
	}
	int getCyclicNext(int cur)
	{
		do
		{
			cur = ___getCyclicNext(cur);
		}
		while(progs[cur] == NULL);
		return cur;
	}
	int ___getCyclicNext(int cur)
	{
		cur++;
		if(cur >= progs.size())
			cur = 0;
		return cur;
	}
	bool checkCPU(int time, int cur)
	{
		Process* p;
		ProcessState* ps;
		ExecutionFragment* ef;

		p = progs[cur];
		ps = p->getState();
		if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
		{
			//
			// Aumentar el contador de la ráfaga de CPU
			//
			ps->cpu++;

			//
			// Si se completó la ráfaga, generar una marca de tiempo
			//
			if(ps->cpu == ef->cpuTime)
			{
				marks.addMark(cur, time+1, TimeMark::TYPE_CPU);

				/*
				if(ef->iowTime == 0)
				{
					ps->next();
				}
				*/
				lastProg = cur;
				cur = -1;
			}
		}
		if(cur != -1)
			lastProg = cur;
		// ...
		return (cur == -1);
	}
};

QString SchedulingFIFO::name() const
{
	return "FIFO";
}
int     SchedulingFIFO::exec(const QVector<Process*>& processList, TimeMarkManager& marks)
{
	int time;
	ContextFIFO ctxt(processList, marks);

	for(time = 0; !ctxt.atEnd(); time++)
	{
		ctxt.checkIO(time);
		ctxt.checkArrivals(time);

		if(-1 != (ctxt.curProg = ctxt.getCurrentProgram(time, ctxt.curProg)))
		{
			if(ctxt.checkCPU(time, ctxt.curProg))
			{
				ctxt.curProg = -1;
			}
		}
		/*
		qDebug("xxxxxxxxxxxx");
		if(time == 30)
			break;
			*/
	}
	//qDebug("time=%d", time);

	return time-1;
}
