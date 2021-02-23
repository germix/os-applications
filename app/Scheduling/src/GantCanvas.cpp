#include "GantCanvas.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QApplication>
#include <QGradient>

#include "GantLine.h"
#include "GantSegment.h"

#define LINES_SPACE 10		// Espacios entre líneas
#define BORDER_SPACE 14		// Espacios extra para los border

static void updateScrollBar(QScrollBar* bar, int max, int page)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(4);
}

GantCanvas::GantCanvas(QWidget* parent) : QAbstractScrollArea(parent)
{
	init();
}
GantCanvas::GantCanvas(const QVector<GantLine*>& lines, int maxTime, QWidget* parent) : QAbstractScrollArea(parent)
{
	this->lines = lines;
	this->maxTime = maxTime;
	init();
}

GantCanvas::~GantCanvas()
{
	deleteLines();
}
void GantCanvas::init()
{
	// ...
	zoom = 30;
	rulerStep = 1;
	offsetX = 0;
	maxTime = 0;
	lineHeight = 20;
	gantWidth = 0;
	gantHeight = 0;

	//Dragger
	mousePressed = false;

	//
	// Inicializar fuente
	//
	font = QApplication::font();
	font.setPointSize(7);
	QFontMetrics fm(font);
	fontHeight = fm.height();

	//
	// Inicializar barras
	//
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	// ...
	setup();
}
void GantCanvas::clear()
{
	maxTime = 0;
	deleteLines();
	setup();
	viewport()->update();
}
void GantCanvas::setLines(const QVector<GantLine*>& lines, int maxTime)
{
	deleteLines();
	this->lines = lines;
	this->maxTime = maxTime;
	setup();
	viewport()->update();
}
void GantCanvas::setRulerStep(int step)
{
	this->rulerStep = step;
	setup();
	viewport()->update();
}
void GantCanvas::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());

	p.fillRect(rect(), Qt::white);

	if(!lines.isEmpty())
	{
		int px = BORDER_SPACE-horizontalScrollBar()->value();
		int py = BORDER_SPACE-verticalScrollBar()->value();
		int x = px;
		int y = py;

		for(int i = 0; i < lines.size(); i++, y += lineHeight+LINES_SPACE)
		{
			drawSegments(p, x+offsetX, y, lines.at(i)->getSegments());
		}
		drawRuler(p, px+offsetX, py, y+BORDER_SPACE-LINES_SPACE, maxTime);
		/*
		p.setPen(Qt::black);
		p.drawRect(0, 0, gantWidth, gantHeight);
		*/
	}
}
void GantCanvas::resizeEvent(QResizeEvent* e)
{
	setup();
}
void GantCanvas::wheelEvent(QWheelEvent* e)
{
	if(e->delta() != 0)
	{
		if(e->modifiers() & Qt::AltModifier)
		{
			if(e->delta() < 0)
				rulerStep--;
			else if(e->delta() > 0)
				rulerStep++;

			if(rulerStep < 1)
				rulerStep = 1;
			else if(rulerStep > 100)
				rulerStep = 100;

			setup();
			viewport()->update();
		}
		else if(e->modifiers() & Qt::ControlModifier)
		{
			if(e->modifiers() & Qt::ShiftModifier)
			{
				if(e->delta() < 0) lineHeight--;
				else if(e->delta() > 0) lineHeight++;

				if(lineHeight < 4) lineHeight = 4;
				else if(lineHeight > 100) lineHeight = 100;
			}
			else
			{
				if(e->delta() < 0) zoom--;
				else if(e->delta() > 0) zoom++;

				if(zoom < 2) zoom = 2;
				else if(zoom > 100) zoom = 100;
			}
			setup();
			viewport()->update();
		}
		else
		{
			QScrollBar* sbr;

			if(e->modifiers() & Qt::ShiftModifier)
				sbr = verticalScrollBar();
			else
				sbr = horizontalScrollBar();

			if(e->delta() < 0)
				sbr->setValue(sbr->value() + sbr->singleStep());
			else if(e->delta() > 0)
				sbr->setValue(sbr->value() - sbr->singleStep());
		}
	}
}
void GantCanvas::mouseMoveEvent(QMouseEvent* e)
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
void GantCanvas::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void GantCanvas::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}
void GantCanvas::setup()
{
	//
	// Calcular el ancho que ocupan las líneas
	//
	gantWidth = maxTime*zoom;
	gantWidth += BORDER_SPACE*2;

	//
	// Calcular la altura que ocupan las líneas
	//
	gantHeight = 0;
	for(int i = 0; i < lines.size(); i++, gantHeight += lineHeight+LINES_SPACE);
	gantHeight -= LINES_SPACE;
	gantHeight += BORDER_SPACE*3;
	gantHeight += fontHeight;

	//
	// Actualizar barras de desplazamiento
	//
	updateScrollBar(horizontalScrollBar(), gantWidth, viewport()->width());
	updateScrollBar(verticalScrollBar(), gantHeight, viewport()->height());
}
void GantCanvas::deleteLines()
{
	for(int i = 0; i < lines.size(); i++)
	{
		delete lines.at(i);
	}
	lines.clear();
}
void GantCanvas::drawRuler(QPainter& p, int x, int y1, int y2, int max)
{
	QFontMetrics fm(font);

	p.setFont(font);
	p.setPen(QPen(Qt::black));
	for(int i = 0; i <= max; i += rulerStep)
	{
		QString txt = QString::number(i);

		p.drawLine(x, y1, x, y2);

		p.drawText(x-fm.width(txt)/2, y2+fm.height(), txt);

		x += rulerStep*zoom;
	}
}
void GantCanvas::drawSegments(QPainter& p, int x, int y, const QVector<GantSegment*>& segments)
{
	for(int i = 0; i < segments.size(); i++)
	{
		GantSegment* seg = segments.at(i);
		int          segWidth = seg->getSize()*zoom;

		p.fillRect(QRect(x,y,segWidth,lineHeight), seg->getColor());

#if 0
		QLinearGradient lg(x, y, x, y+lineHeight/2);
		lg.setColorAt(0, QColor(255, 255, 255, 180));
		lg.setColorAt(1, QColor(255, 255, 255, 0));
		p.fillRect(QRect(x,y,segWidth,lineHeight), lg);
#else
		QLinearGradient lg(x, y, x, y+(double)lineHeight/1.5);
		lg.setColorAt(0, QColor(255, 255, 255, 110));
		lg.setColorAt(1, QColor(255, 255, 255, 0));
		p.fillRect(QRect(x,y,segWidth,lineHeight), lg);

		QLinearGradient lg2(x, y+lineHeight-(double)lineHeight/2.5, x, y+lineHeight);
		lg2.setColorAt(0, QColor(0, 0, 0, 0));
		lg2.setColorAt(1, QColor(0, 0, 0, 50));
		p.fillRect(QRect(x,y,segWidth,lineHeight), lg2);
#endif
		x += segWidth;
	}
}


