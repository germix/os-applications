#include "BuddySystemCanvas2.h"

#include "BuddyNode.h"
#include "BuddySystem.h"

#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>

QString formatBytesToSize(float bytes);

#define FRAME_HEIGHT		40

#define PADDING_SIZE		20

#define USE_SCROLLBARS		false

static int scrollOffset(QScrollBar* bar, int tableSize, int viewportSize)
{
	if((viewportSize-PADDING_SIZE*2)>tableSize)
	{
		return (viewportSize/2 - tableSize/2);
	}
	return -bar->value() + PADDING_SIZE;
}
static void updateScrollBar(QScrollBar* bar, int max, int page)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(4);
}


static void paintNode(QPainter& p, BuddyNode* node, double x, double y, double width, int height)
{
	if(!node)
		return;

	double half = width/2;
	QColor usedColor(153, 217, 234);
	QColor emptyColor(230, 230, 230);


	paintNode(p, node->getLeft(),  x     , y, half, height);
	paintNode(p, node->getRight(), x+half, y, half, height);

	if(!node->isSplitted())
	{
		QRectF rect(x,y,width,height);

		if(node->isUsed())
			p.fillRect(rect, usedColor);
		else
			p.fillRect(rect, emptyColor);
		p.drawRect(rect);

		QFontMetrics fm(p.font());
		QString text = node->getName();
		if(!text.isEmpty())
			text += " = ";
		text += formatBytesToSize(node->getSize());
		int     textWidth = fm.width(text);
		int     textHeight = fm.height();

		p.setClipping(true);
		p.setClipRect(rect);
		p.drawText(
				x + (width-textWidth)/2,
				y + (height-textHeight)/2+fm.ascent(), text);

		p.setClipping(false);
	}
}

BuddySystemCanvas2::BuddySystemCanvas2(BuddySystem* sys, QWidget* parent) : QAbstractScrollArea(parent), system(sys)
{
	zoom = 1;
	mousePressed = false;

	setMinimumHeight(FRAME_HEIGHT+(PADDING_SIZE*2)+30);
	setMaximumHeight(FRAME_HEIGHT+(PADDING_SIZE*2)+30);

#if USE_SCROLLBARS
#else
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif
}
BuddySystemCanvas2::~BuddySystemCanvas2()
{
}
void BuddySystemCanvas2::rebuild()
{
	viewport()->update();
}
void BuddySystemCanvas2::paintEvent(QPaintEvent* e)
{
	QPainter p(this->viewport());
	//
	// Pintar fondo
	//
	p.fillRect(QRect(0, 0, width(), height()), Qt::white);

	//
	// Pintar celdas
	//
	int viewportWidth = width();
	int viewportHeight = height();
#if USE_SCROLLBARS
	int sx = scrollOffset(horizontalScrollBar(), boxWidth, viewportWidth);
	int sy = scrollOffset(verticalScrollBar()  , boxHeight, viewportHeight);
	p.translate(sx, sy);
#else

	int sx = scrollOffset(horizontalScrollBar(), boxWidth, viewportWidth);
	int sy = scrollOffset(verticalScrollBar()  , boxHeight, viewportHeight);
	/*
	int sy = (viewportHeight-boxHeight)/2 + dy;
	int sx = (viewportWidth-boxWidth)/2 + dx;
*/
#endif
	p.setFont(QFont("Arial", 8));
	p.translate(sx, sy);
	paintNode(p, system->getRoot(), 0, 0, boxWidth, boxHeight);
}
void BuddySystemCanvas2::wheelEvent(QWheelEvent* e)
{
	if(e->modifiers() & Qt::ControlModifier)
	{
		if(e->delta() < 0)
			zoom /= 1.1;
		else
			zoom *= 1.1;
		zoom = qBound(0.1, zoom, 100.);

		updateScrollBars();
		viewport()->update();
	}
}
void BuddySystemCanvas2::resizeEvent(QResizeEvent* e)
{
	updateScrollBars();
}
void BuddySystemCanvas2::keyPressEvent(QKeyEvent* e){}
void BuddySystemCanvas2::keyReleaseEvent(QKeyEvent* e){}
void BuddySystemCanvas2::mouseMoveEvent(QMouseEvent* e)
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
void BuddySystemCanvas2::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void BuddySystemCanvas2::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}
void BuddySystemCanvas2::updateScrollBars()
{
	boxWidth = qMax(100,viewport()->width()-100)*zoom;
	boxHeight = FRAME_HEIGHT;

	updateScrollBar(horizontalScrollBar(), boxWidth+(PADDING_SIZE*2), viewport()->width());
	updateScrollBar(verticalScrollBar(),   boxHeight+(PADDING_SIZE*2), viewport()->height());
}

