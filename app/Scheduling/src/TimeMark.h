#ifndef TIMEMARK_H
#define TIMEMARK_H

class TimeMark
{
	int time;
	int type;
public:
	enum
	{
		TYPE_CPU,
		TYPE_IO,
		TYPE_WAIT,
		TYPE_UNLOADED,
	};
public:
	TimeMark()
	{
	}
	TimeMark(int time, int type)
	{
		this->time = time;
		this->type = type;
	}
public:
	int getTime() const { return time; }
	int getType() const { return type; }
};

#endif // TIMEMARK_H
