#include "WorkingSetCanvas.h"
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>

#define SPACE 10


#define PADDING_SIZE		10

static void updateScrollBar(QScrollBar* bar, int max, int page)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(4);
}
static int scrollOffset(QScrollBar* bar, int tableSize, int viewportSize)
{
	if((viewportSize-PADDING_SIZE*2)>tableSize)
	{
		return (viewportSize/2 - tableSize/2);
	}
	return -bar->value() + PADDING_SIZE;
}
static void drawArrowHead(QPainter& p, int x, int y, int dx, int dy)
{
	QPainterPath path;
	path.moveTo(x, y);
	path.lineTo(x+dx, y+dy);
	path.lineTo(x, y+dy);
	path.lineTo(x, y);
	p.fillPath(path, Qt::black);
}

WorkingSetCanvas::WorkingSetCanvas(QWidget *parent) : QAbstractScrollArea(parent)
{
	sequenceWidth = 0;

	font = QFont("Courier", 14);
	fontHeight = QFontMetrics(font).xHeight();

	updateScrollBars();
}
WorkingSetCanvas::~WorkingSetCanvas()
{
}

void WorkingSetCanvas::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());
	if(sequence.isEmpty())
	{
		return;
	}
	p.setFont(font);
	QFontMetrics fm(font);
/*
	p.setRenderHints(QPainter::Antialiasing, true);
	p.setRenderHints(QPainter::TextAntialiasing, true);
*/
	int vw = width();
	int sx = scrollOffset(horizontalScrollBar(), sequenceWidth, vw);
	//
	// ...
	//
	p.translate(sx, viewport()->height()/2 - fontHeight/2);
//	p.translate(viewport()->width()/2 - sequenceWidth/2, viewport()->height()/2 - fontHeight/2);
	//
	// ...
	//
	int x = 0;
	int y = fontHeight + fm.ascent();

	int xTail = 0;
	int xCurrent = 0;
	for(int i = 0; i < sequence.size(); i++)
	{
		int num = sequence.at(i);
		QString txt = QString::number(num);
		int width = fm.width(txt);

		p.drawText(x, y, txt);

		if(i == tail) xTail = x;
		if(i == current) xCurrent = x + width;

		x += width + SPACE;
	}
	//
	// Límites de la ventana
	//
	int lineY1 = y + fontHeight;
	int lineY2 = lineY1+12;
	p.drawLine(xTail, lineY2, xCurrent, lineY2);
	p.drawLine(xTail, lineY1, xTail, lineY2);
	p.drawLine(xCurrent, lineY1, xCurrent, lineY2);

	drawArrowHead(p, xTail, lineY1, -3, 4);
	drawArrowHead(p, xCurrent, lineY1, +3, 4);
}
void WorkingSetCanvas::resizeEvent(QResizeEvent* e)
{
	updateScrollBars();
}
void WorkingSetCanvas::wheelEvent(QWheelEvent* e)
{
	if(e->delta() != 0)
	{
		QScrollBar* sbr = horizontalScrollBar();

		if(e->delta() < 0)
			sbr->setValue(sbr->value() + sbr->singleStep());
		else if(e->delta() > 0)
			sbr->setValue(sbr->value() - sbr->singleStep());
	}
}
void WorkingSetCanvas::mouseMoveEvent(QMouseEvent* e)
{
	if(mousePressed)
	{
		int dx = e->pos().x()-refPos.x();
		int dy = e->pos().y()-refPos.y();
		QScrollBar* hsb = horizontalScrollBar();
		QScrollBar* vsb = verticalScrollBar();

		refPos = e->pos();
		hsb->setValue(hsb->value()-dx);
		vsb->setValue(vsb->value()-dy);
	}
}
void WorkingSetCanvas::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void WorkingSetCanvas::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}
void WorkingSetCanvas::clear()
{
	wsize = -1;
	sequence.clear();
	sequenceWidth = 0;
	updateScrollBars();
	viewport()->update();
}
void WorkingSetCanvas::init(const QVector<int>& seq, int wsize)
{
	this->wsize = wsize;
	this->sequence = seq;
	this->viewport()->update();

	// Limites
	tail = 0;
	current = 0;

	//
	// Calcular el ancho de toda la sequencia
	//
	QFontMetrics fm(font);
	sequenceWidth = 0;
	for(int i = 0; i < sequence.size(); i++)
	{
		sequenceWidth += fm.width(QString::number(sequence.at(i)));
		if((i+1) != sequence.size())
		{
			sequenceWidth += SPACE;
		}
	}
	this->updateScrollBars();
}
void WorkingSetCanvas::step(const QVector<PageWS *> &wset, int tail, int current)
{
	this->wset = wset;
	this->tail = tail;
	this->current = current;
	this->viewport()->update();
}
void WorkingSetCanvas::updateScrollBars()
{
	updateScrollBar(horizontalScrollBar(), sequenceWidth+(PADDING_SIZE*2), viewport()->width());
}



