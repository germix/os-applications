#include "Canvas.h"
#include <QPainter>
#include <QHash>
#include <QMouseEvent>
#include <QScrollBar>

#include "MemoryManager.h"

#define PADDING 10


QString formatBytesToSize(float bytes);

static void updateScrollBar(QScrollBar* bar, int max, int page, int step)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(step);
}

Canvas::Canvas(MemoryManager* mmgr__, QWidget *parent) : QAbstractScrollArea(parent), mmgr(mmgr__), zoom(1)
{
}
Canvas::~Canvas()
{
}
void Canvas::setup()
{
	maxWidth = (viewport()->width() - PADDING*2)*zoom;
	updateScrollBar(horizontalScrollBar(), maxWidth+(PADDING*2)-1, viewport()->width(), 16);
}
void Canvas::reset()
{
	zoom = 1;
	setup();
	viewport()->update();
}
void Canvas::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());
	double w = viewport()->width();
	double h = viewport()->height();
	int boxHeight = 60;

	p.fillRect(0, 0, w, h, Qt::white);

	int memSize = mmgr->getSize();

	p.setFont(QFont("Arial", 7));

	p.translate(-horizontalScrollBar()->value(), 0);
	if(memSize > 0)
	{
		for(Chunk* c = mmgr->getHead(); c != NULL; c = c->next)
		{
			double x = ((c->pos/(double)memSize)*maxWidth)+PADDING;
			double s = ((c->size/(double)memSize)*maxWidth);
			QRectF rc(x, h/2 - boxHeight/2, s, boxHeight);

			if(c->used)
			{
				//p.fillRect(rc, QColor(200, 200, 255));
				if(c->color.isValid())
					p.fillRect(rc, c->color);
				else
					p.fillRect(rc, QColor(130, 130, 130));
			}
			p.drawRect(rc);
			// ...
			QFontMetrics fm(p.font());
			QString text = formatBytesToSize(c->size);
			int     textWidth = fm.width(text);
#if 0
			int     textHeight = fm.height();
			p.drawText(rc.x() + (rc.width() - textWidth)/2,
						rc.y() + (rc.height() - textHeight)/2+fm.ascent(), text);
#else
			p.drawText(rc.x() + (rc.width() - textWidth)/2,
						rc.y() + rc.height() + fm.ascent(), text);
#endif
		}
	}
}
void Canvas::wheelEvent(QWheelEvent* e)
{
	if(e->modifiers() & Qt::ControlModifier)
	{
		if(e->delta() < 0) zoom -= 0.2;
		else if(e->delta() > 0) zoom += 0.2;

		if(zoom < 1) zoom = 1;
		else if(zoom > 100) zoom = 100;

		setup();
		viewport()->update();
	}
	else
	{
		QScrollBar* sbr = horizontalScrollBar();

		if(e->delta() < 0)
			sbr->setValue(sbr->value() + sbr->singleStep());
		else if(e->delta() > 0)
			sbr->setValue(sbr->value() - sbr->singleStep());
	}
}
void Canvas::resizeEvent(QResizeEvent* e)
{
	setup();
}
void Canvas::mouseMoveEvent(QMouseEvent* e)
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
void Canvas::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void Canvas::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}

