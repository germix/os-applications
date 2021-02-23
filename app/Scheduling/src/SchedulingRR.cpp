#include "SchedulingRR.h"

#include "AbstractContext.h"
#include "TimeMarkManager.h"
#include <QQueue>

class ContextRR : public AbstractContext
{
public:
	QQueue<Process*>	queue;
	int					quantum;

	Process*			lastProgP;
	Process*			postAppend;
public:
	ContextRR(int q, const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext(p, m), quantum(q)
	{
		lastProgP = NULL;
		postAppend = NULL;
	}
	~ContextRR()
	{
	}
private:
	int getCur__()
	{
		if(!queue.isEmpty())
		{
			Process* p = queue.head();

			/*
			while(p->isIoWaiting())
			{
				queue.enqueue(queue.dequeue());
				p = queue.head();
			}
*/
			for(int i = 0; i < progs.size(); i++)
			{
				if(p == progs[i])
				{
					return i;
				}
			}
		}
		return -1;
	}
	int getCur()
	{
		int cur = getCur__();
		if(cur != -1)
		{
			int firstCur = cur;
			do
			{
				if(progs[cur]->isIoWaiting())
				{
					queue.enqueue(queue.dequeue());
				}
				cur = getCur__();
				if(cur == -1 || firstCur == cur)
					break;
			}
			while(progs[cur]->isIoWaiting());
		}
		return cur;
	}
	void getCurrentProgram(int time)
	{
#if 0
		if(false)
		{
			if(cur != -1)
			{
				ProgramState ps = states[cur];
				ExecutionFragment ef = states[cur].getFragment();

				if(ps.cpu < ef.burstTime)
				{
					if(ps.cpu != 0 && (ps.cpu % quantum) == 0)
					{
						marks[cur].add(new TimeMark(time, TimeMark.TYPE_CPU));

						/*
						if(ef.ioTime == 0)
						{
							states[cur].next();
						}
						*/
						lastProg = cur;
						cur = -1;
					}
				}
			}
			if(cur == -1)
			{
				int p = lastProg;
				ProgramState ps;
				ExecutionFragment ef;

				while(lastProg != (p = getCyclicNext(p)))
				{
					ps = states[p];
					if((null != (ef = ps.getFragment())) && (ps.cpu < ef.burstTime))
					{
						marks[p].add(new TimeMark(time, TimeMark.TYPE_WAIT));
						return p;
					}
				}
				if(p == lastProg)
				{
					ps = states[p];
					if((null != (ef = ps.getFragment())) && (ps.cpu < ef.burstTime))
					{
						return p;
					}
				}
				return -1;
			}
		}
		else
#endif
		{
			int cur;
			if(-1 != (cur = getCur()))
			{
				Process* p = progs[cur];
				ProcessState* ps = p->getState();
				ExecutionFragment* ef = ps->getFragment();

				if(ef != NULL)
				{
					if(p != lastProgP)
					{
						if(!p->isIoWaiting())
						marks.addMark(cur, time, TimeMark::TYPE_WAIT);
					}
#if 0
					if(ps->cpu == ef->cpuTime)
					{
						marks.addMark(cur, time+1, TimeMark::TYPE_CPU);

						if(ef->iowTime == 0)
						{
							ps->next();
						}
						queue.dequeue();
					}
					else
					{
						ps->cpu++;

						if(ps->cpu < ef->cpuTime && (ps->cpu % quantum) == 0)
						{
							marks.addMark(cur, time+1, TimeMark::TYPE_CPU);
							if(ps->cpu < ef->cpuTime)
							{
								//showQueue();
								//queue.add(queue.poll());

//								postAppend = queue.poll();
								postAppend = queue.dequeue();
								//showQueue();
							}
						}
					}
#else
					if(ps->cpu < ef->cpuTime)
					{
						ps->cpu++;
						if(ps->cpu == ef->cpuTime)
						{
							//qDebug("0000000000");
							marks.addMark(cur, time+1, TimeMark::TYPE_CPU);
							if(ef->iowTime == 0)
							{
								//ps->next();
								queue.dequeue();
							}
							else
							{
								postAppend = queue.dequeue();
							}
						}
						else if(ps->cpu != 0 && (ps->cpu % quantum) == 0)
						{
							marks.addMark(cur, time+1, TimeMark::TYPE_CPU);
							postAppend = queue.dequeue();
						}
					}
#endif
				}
				lastProgP = p;
			}
		}
	}
protected:
	virtual void onBurstTime(int time)
	{
		if(postAppend != NULL)
		{
			queue.enqueue(postAppend);
			postAppend = NULL;
		}
		//ctxt.showQueue(time);

		//ctxt.showQueue();

		//if(-1 != (cur = ctxt.getCurrentProgram(time, cur)))
		getCurrentProgram(time);
	}
	virtual void onArrivedProgram(int time, int index, Process* prog)
	{
		if(prog->getState()->getFragment()->cpuTime > 0)
		{
			queue.enqueue(prog);
		}
	}
};

QString SchedulingRR::name() const
{
	return "RR";
}
#include <QInputDialog>

int     SchedulingRR::exec(const QVector<Process*>& processList, TimeMarkManager& marks)
{
#if 0
	return ContextRR(2, processList, marks).run();
#else
	QString quantum = QInputDialog::getText(NULL, "Quantum", "Quantum:");
	bool ok = false;
	int quantomInt = quantum.toInt(&ok);
	if(ok)
	{
		return ContextRR(quantomInt, processList, marks).run();
	}
	return -1;
#endif
}
