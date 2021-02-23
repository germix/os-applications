#ifndef TIMEMARKMANAGER_H
#define TIMEMARKMANAGER_H
#include <QVector>

#include "TimeMark.h"

class GantLine;

class TimeMarkList : public QVector<TimeMark>
{
public:
};

class TimeMarkManager
{
	QVector<TimeMarkList> marks;
public:
	TimeMarkManager(int max);
	~TimeMarkManager();
public:
	void addMark(int index, int time, int type);
	QVector<GantLine*> toGantLines() const;
};

#endif // TIMEMARKMANAGER_H
