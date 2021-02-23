#include "SchedulingSJF.h"

#include "AbstractContext.h"
#include "TimeMarkManager.h"

class ContextSJF : public AbstractContext
{
public:
	int curProg;
	int lastProg;
public:
	ContextSJF(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext(p, m)
	{
		curProg = 0;
		lastProg = 0;
	}
	~ContextSJF()
	{
	}
private:
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
	int getCurrentProgram(int time, int cur)
	{
		if(cur == -1)
		{
			int sel = -1;
			int min = INT_MAX;
			Process* p;
			ProcessState* ps;
			ExecutionFragment* ef;

			//
			// TODO: Comprobar programas activos no cargados
			//
			for(int i = 0; i < progs.size(); i++)
			{
				p = progs[i];
				if(p != NULL)
				{
					ps = p->getState();
					if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
					{
						if((ef->cpuTime-ps->cpu) < min)
						{
							sel = i;
							min = ef->cpuTime-ps->cpu;
						}
					}
				}
			}
			if(sel != -1)
			{
				marks.addMark(sel, time, TimeMark::TYPE_WAIT);
				return sel;
			}
		}
		return cur;
	}
protected:
	virtual void onBurstTime(int time)
	{
		if(-1 != (curProg = getCurrentProgram(time, curProg)))
		{
			if(checkCPU(time, curProg))
			{
				curProg = -1;
			}
		}
	}
	virtual void onArrivedProgram(int index, int time, Process* prog)
	{
	}
};

QString SchedulingSJF::name() const
{
	return "SJF";
}
int     SchedulingSJF::exec(const QVector<Process*>& processList, TimeMarkManager& marks)
{
	return ContextSJF(processList, marks).run();
}

