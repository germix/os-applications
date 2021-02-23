#ifndef GANTLINE_H
#define GANTLINE_H
#include <QVector>
#include "GantSegment.h"

class GantLine
{
	QVector<GantSegment*> segments;
public:
	GantLine()
	{
	}
	~GantLine()
	{
		for(int i = 0; i < segments.size(); i++)
		{
			delete segments.at(i);
		}
		segments.clear();
	}
public:
	const QVector<GantSegment*>& getSegments() const
	{
		return segments;
	}
	void addSegment(int size, const QColor& color)
	{
		segments.append(new GantSegment(size, color));
	}
};

#endif // GANTLINE_H
