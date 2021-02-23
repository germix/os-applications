#include "AbstractContext.h"

#include "TimeMarkManager.h"

AbstractContext::AbstractContext(const QVector<Process*>& p, TimeMarkManager& m) : marks(m)
{
	endFlag = false;

	progs.resize(p.size());
	for(int i = 0; i < p.size(); i++)
		progs[i] = NULL;

	allProgs = p;
}
AbstractContext::~AbstractContext()
{

}
void AbstractContext::checkIO(int time)
{
	int finishedProcessCount = 0;

	//
	// Decrementar tiempo de E/S si no hay tiempo de ráfaga de CPU
	//
	for(int i = 0; i < progs.size(); i++)
	{
		Process* p;
		ProcessState* ps;
		ExecutionFragment* ef;

		p = progs[i];
		if(p != NULL)
		{
			ps = p->getState();
			if(NULL != (ef = ps->getFragment()))
			{
				if((ps->cpu == ef->cpuTime) && ps->iow < ef->iowTime)
				{
					if(++ps->iow == ef->iowTime)
					{
						if(ef->iowTime > 0)
						{
							marks.addMark(i, time, TimeMark::TYPE_IO);
						}
						ps->next();
					}
				}
			}
			if(ps->isFinished())
			{
				//finishedProcessCount++;
			}
		}
	}
	for(int i = 0; i < allProgs.size(); i++)
	{
		if(allProgs[i]->isFinished())
		{
			finishedProcessCount++;
		}
	}
	//qDebug("finishedProcessCount=%d", finishedProcessCount);
	if(finishedProcessCount == progs.size())
	{
		endFlag = true;
	}
}
void AbstractContext::checkArrivals(int time)
{
	//
	// Comprobar llegada de procesos
	//
	for(int i = 0; i < allProgs.size(); i++)
	{
		if(time == allProgs[i]->getArrivalTime())
		{
			ExecutionFragment* ef;

			//if(NULL != (ef = allProgs[i]->getState()->getFragment()))
			{
				//if(ef->cpuTime > 0)
				{
					progs[i] = allProgs[i];
					onArrivedProgram(time, i, allProgs[i]);
					if(time > 0)
					{
						marks.addMark(i, time, TimeMark::TYPE_UNLOADED);
					}
				}
			}
		}
	}
}

