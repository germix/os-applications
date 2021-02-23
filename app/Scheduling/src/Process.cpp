#include "Process.h"
#include <QString>
#include <QStringList>

Process::Process(int id, int at, const QString& efs) : state(this)
{
	this->id = id;
	this->arrival = at;
	if(efs.isEmpty())
	{
		fragments = new ExecutionFragment[1];
		fragments[0].cpuTime = 0;
		fragments[0].iowTime = 0;
		fragmentsCount = 1;
	}
	else
	{
		QStringList l = efs.split(",");
		int n = (l.length()+(l.length()&1))/2;

		fragments = new ExecutionFragment[n];
		for(int i = 0; i < n; i++)
		{
			fragments[i].cpuTime = l.at((i*2)+0).toInt();
			fragments[i].iowTime = ((i*2)+1 < l.length()) ? l.at((i*2)+1).toInt() : 0;
		}
		fragmentsCount = n;
	}
}
Process::~Process()
{
	if(fragments != NULL)
		delete [] fragments;
}
bool Process::isFinished() const
{
	return state.isFinished();
}
bool Process::isIoWaiting() const
{
	ExecutionFragment* ef;
	if(NULL != (ef = state.getFragment()))
	{
		if(ef->cpuTime > 0 && ef->cpuTime == state.cpu)
		{
			return true;
		}
	}
	return false;
}
