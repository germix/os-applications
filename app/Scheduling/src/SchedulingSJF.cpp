#include "SchedulingSJF.h"

#include "AbstractContext.h"
#include "TimeMarkManager.h"

#include "AbstractContext2.h"

class ContextSJF : public AbstractContext2
{
public:
	int cur;
public:
	ContextSJF(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext2(p, m), cur(-1)
	{
	}
	~ContextSJF()
	{
	}
public:
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
			for(int i = 0; i < queueCpu.size(); i++)
			{
				p = allProgs[queueCpu.at(i)];
				if(p != NULL)
				{
					ps = p->getState();
					if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
					{
						if((ef->cpuTime-ps->cpu) < min)
						{
							sel = queueCpu.at(i);
							min = ef->cpuTime-ps->cpu;
						}
					}
				}
			}
			if(sel != -1)
			{
				qDebug("sel=%d", sel);
				marks.addMark(sel, time, TimeMark::TYPE_WAIT);
				return sel;
			}
		}
		return cur;
	}
	virtual int onCurrentProgram(int time)
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
			for(int i = 0; i < queueCpu.size(); i++)
			{
				p = allProgs[queueCpu.at(i)];
				if(p != NULL)
				{
					ps = p->getState();
					if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
					{
						if((ef->cpuTime-ps->cpu) < min)
						{
							sel = queueCpu.at(i);
							min = ef->cpuTime-ps->cpu;
						}
					}
				}
			}
			if(sel != -1)
			{
				cur = sel;
				marks.addMark(sel, time, TimeMark::TYPE_WAIT);
				//return sel;
			}
		}
		return cur;
	}
	virtual void onBurstTimeFinished(int time, int index, Process *prog)
	{
		cur = -1;
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




