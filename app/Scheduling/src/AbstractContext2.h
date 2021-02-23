#ifndef ABSTRACTCONTEXT2_H
#define ABSTRACTCONTEXT2_H
#include "AbstractContext.h"

#include <QQueue>
#include "TimeMarkManager.h"


class AbstractContext2 : public AbstractContext
{
public:
	QQueue<int>		queueIo;
	QQueue<int>		queueCpu;
public:
	AbstractContext2(const QVector<Process*>& p, TimeMarkManager& m) : AbstractContext(p, m)
	{
	}
	~AbstractContext2()
	{
	}
	virtual void onBurstTime(int time)
	{
		int cur;
		if(-1 != (cur = onCurrentProgram(time)))
		{
			if(checkCPU(time, cur))
			{
			}
		}
	}
	void checkIO(int time)
	{
		int finishedProcessCount = 0;

		//
		// Decrementar tiempo de E/S si no hay tiempo de r√°faga de CPU
		//
		QList<int> removables;
		for(int i = 0; i < queueIo.size(); i++)
		{
			int idx = queueIo.at(i);
			Process* p = allProgs[idx];
			ProcessState* ps = p->getState();
			ExecutionFragment* ef = ps->getFragment();

			qDebug("ef=%x", ef);
			if(NULL != ef)
			{
				if(ps->iow < ef->iowTime)
				{
					if(++ps->iow == ef->iowTime)
					{
						if(ef->iowTime > 0)
						{
							marks.addMark(idx, time, TimeMark::TYPE_IO);
						}
						ps->next();

						removables.append(idx);

						onIoTimeFinished(time, idx, p);
					}
				}
			}
		}
		for(int i = 0; i < removables.size(); i++)
		{
			queueIo.removeOne(removables.at(i));
		}
		for(int i = 0; i < allProgs.size(); i++)
		{
			if(allProgs[i]->isFinished())
			{
				finishedProcessCount++;
			}
		}
		//qDebug("finishedProcessCount=%d", finishedProcessCount);
		if(finishedProcessCount == allProgs.size())
		{
			endFlag = true;
		}
	}
	bool checkCPU(int time, int cur)
	{
		Process* p;
		ProcessState* ps;
		ExecutionFragment* ef;

		p = allProgs[cur];
		ps = p->getState();
		if((NULL != (ef = ps->getFragment())) && (ps->cpu < ef->cpuTime))
		{
			//
			// Aumentar el contador de la r·faga de CPU
			//
			ps->cpu++;

			//
			// Si se completÛ la r·faga, generar una marca de tiempo
			//
			if(ps->cpu == ef->cpuTime)
			{
				marks.addMark(cur, time+1, TimeMark::TYPE_CPU);

				if(ef->iowTime > 0)
				{
					queueIo.enqueue(cur);
				}
				else if(ef->iowTime == 0)
				{
					ps->next();
				}

				onBurstTimeFinished(time, cur, p);
				return true;
			}
		}
		return false;
	}
	virtual int onCurrentProgram(int time)
	{
	}
	virtual void onArrivedProgram(int time, int index, Process* prog)
	{
		qDebug("arrived time=%d, index=%d", time, index);
		queueCpu.enqueue(index);
	}
	virtual void onIoTimeFinished(int time, int index, Process* prog)
	{
	}
	virtual void onBurstTimeFinished(int time, int index, Process* prog)
	{
	}
};

#endif // ABSTRACTCONTEXT2_H
