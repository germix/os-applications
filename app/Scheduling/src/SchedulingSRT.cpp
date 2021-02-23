#include "SchedulingSRT.h"

#include "AbstractContext.h"
#include "AbstractContext2.h"

class ContextSRT : public AbstractContext2
{
public:
	int cur;
public:
	ContextSRT(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext2(p, m), cur(-1)
	{
	}
	~ContextSRT()
	{
	}
private:
	virtual int onCurrentProgram(int time)
	{
		int sel = -1;
		int min = INT_MAX;
		Process* p;
		ProcessState* ps;
		ExecutionFragment* ef;

		for(int i = 0; i < progs.size(); i++)
		{
			if(NULL != (p = progs[i]))
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
			if(cur == -1)
			{
				marks.addMark(sel, time, TimeMark::TYPE_WAIT);
			}
			else
			{
				marks.addMark(cur, time, TimeMark::TYPE_CPU);
			}
			cur = sel;
		}
		return cur;
	}
	virtual void onBurstTimeFinished(int time, int index, Process* prog)
	{
		cur = -1;
	}
};

QString SchedulingSRT::name() const
{
	return "SRT";
}
int     SchedulingSRT::exec(const QVector<Process*>& processList, TimeMarkManager& marks)
{
	return ContextSRT(processList, marks).run();
}
