#ifndef GANTSEGMENT_H
#define GANTSEGMENT_H
#include <QColor>

class GantSegment
{
	int size;
	QColor color;
public:
	GantSegment(int size, const QColor& color)
	{
		this->size = size;
		this->color = color;
	}
	~GantSegment()
	{
	}
public:
	int getSize() const { return size; }
	QColor getColor() const { return color; }
};

#endif // GANTSEGMENT_H
