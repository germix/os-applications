#ifndef PROCESS_H
#define PROCESS_H

class QString;
class Process;

class ExecutionFragment
{
public:
	int cpuTime;		// Tiempo de ráfaga de uso de CPU
	int iowTime;		// Tiempo de espera en las operaciones de E/S
};
class ProcessState
{
public:
	int cpu;
	int iow;
private:
	int ief;
	Process* prog;
public:
	ProcessState(Process* p);
	~ProcessState();
public:
	void next()
	{
		ief++;
		cpu = 0;
		iow = -1;
	}
	bool isFinished() const;
	ExecutionFragment* getFragment() const;

};

class Process
{
	int					id;			// Identificador
	int					arrival;	// Tiempo de llegada
	ExecutionFragment*	fragments;
	int					fragmentsCount;
	ProcessState		state;

	friend class ProcessState;
public:
	Process(int id, int at, const QString& efs);
	~Process();
public:
	ProcessState* getState()
	{
		return &state;
	}
	int getArrivalTime() const
	{
		return arrival;
	}
	bool isFinished() const;
	bool isIoWaiting() const;
};

#endif // PROCESS_H
