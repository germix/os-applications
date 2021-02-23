#include "TableElement.h"
#include <QPainter>
#include <QFontMetrics>

void TableElement::paint(QPainter& p, const QFontMetrics& fm, int x, int y, int w, int h)
{
	p.fillRect(QRect(x,y,w,h), backColor);

	if(!text.isEmpty())
	{
		int txtw = fm.width(text);
		int txth = fm.height();

		p.setPen(QPen(textColor));
		p.drawText(x + (w/2 - txtw/2), y+fm.ascent() + (h/2 - txth/2), text);
	}
}
void TableElement::paint(QPainter& p, const QFont& f1, const QFontMetrics& fm1, const QFont& f2, const QFontMetrics& fm2, int x, int y, int w, int h)
{
	p.fillRect(QRect(x,y,w,h), backColor);

	if(!text.isEmpty())
	{
		int txtw = fm1.width(text);
		int txth = fm1.height();

		p.setPen(QPen(textColor));
		p.setFont(f1);
		p.drawText(x + (w/2 - txtw/2), y+fm1.ascent() + (h/2 - txth/2), text);
	}
	if(!bitText.isEmpty())
	{
		p.setPen(QPen(textColor));
		p.setFont(f2);
		p.drawText(x + (w-fm2.width(bitText)-2), y + fm2.ascent() + 2, bitText);
	}
}
