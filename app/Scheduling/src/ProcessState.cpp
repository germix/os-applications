#include "Process.h"

ProcessState::ProcessState(Process* p) : cpu(0),iow(-1),ief(0),prog(p)
{
}
ProcessState::~ProcessState()
{
}
bool ProcessState::isFinished() const
{
	if(ief < prog->fragmentsCount)
	{
		return false;
	}
	return true;
}
ExecutionFragment* ProcessState::getFragment() const
{
	if(ief < prog->fragmentsCount)
	{
		return &prog->fragments[ief];
	}
	return 0;
}

