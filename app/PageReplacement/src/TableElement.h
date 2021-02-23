#ifndef TABLEELEMENT_H
#define TABLEELEMENT_H
#include <QColor>
#include <QString>

class QPainter;
class QFont;
class QFontMetrics;

class TableElement
{
	QString		text;
	QColor		backColor;
	QColor		textColor;
	QString		bitText;
public:
	TableElement(const QColor& clr)
	{
		backColor = clr;
	}
	TableElement(int n, const QColor& clr)
	{
		text = QString().sprintf("%d", n);
		backColor = clr;
		textColor = Qt::black;
	}
	TableElement(const QString& txt, const QColor& clr)
	{
		text = txt;
		backColor = clr;
		textColor = Qt::black;
	}
	TableElement(const QString& txt, const QColor& bkclr, const QColor& txtclr)
	{
		text = txt;
		backColor = bkclr;
		textColor = txtclr;
	}
	TableElement(const QString& txt, const QColor& bkclr, const QColor& txtclr, const QString& bit)
	{
		text = txt;
		backColor = bkclr;
		textColor = txtclr;
		bitText = bit;
	}
	~TableElement()
	{
	}
public:
	void paint(QPainter& p, const QFontMetrics& fm, int x, int y, int w, int h);
	void paint(QPainter& p, const QFont& f1, const QFontMetrics& fm1, const QFont& f2, const QFontMetrics& fm2, int x, int y, int w, int h);
};

#endif // TABLEELEMENT_H
