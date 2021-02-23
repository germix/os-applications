#include "TimeMarkManager.h"

#include "GantLine.h"
#include "GantSegment.h"

TimeMarkManager::TimeMarkManager(int max)
{
	marks.resize(max);
}
TimeMarkManager::~TimeMarkManager()
{
}
void TimeMarkManager::addMark(int index, int time, int type)
{
	marks[index].append(TimeMark(time, type));
}
QVector<GantLine*> TimeMarkManager::toGantLines() const
{
	QVector<GantLine*> lines;

	for(int i = 0; i < marks.size(); i++)
	{
		const TimeMarkList& m = marks.at(i);
		GantLine* line = new GantLine();

		lines.append(line);

		int x = 0;
		for(int j = 0; j < m.size(); j++)
		{
			int tim = m.at(j).getTime();
			int typ = m.at(j).getType();
			QColor col;
			if(typ == TimeMark::TYPE_CPU) col = Qt::red;
			else if(typ == TimeMark::TYPE_IO) col = Qt::blue;
			else if(typ == TimeMark::TYPE_WAIT) col = Qt::yellow;
			else if(typ == TimeMark::TYPE_UNLOADED) col = Qt::lightGray;
			else col = Qt::black;

			int s = tim-x;
			if(s > 0)
			{
				line->addSegment(tim-x, col);

				//System.out.println("x="+x);
				//System.out.println("tim="+tim);
				//System.out.println("aaaaa="+(tim-x));
				x = tim;
			}
			else
			{
				//System.out.println("tim="+tim+" typ="+typ+" s="+s);
			}
		}
	}
	return lines;
}

