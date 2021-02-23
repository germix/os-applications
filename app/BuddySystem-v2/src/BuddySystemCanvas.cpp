#include "BuddySystemCanvas.h"

#include "BuddyNode.h"
#include "BuddySystem.h"

#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>

#define NODE_SIZE			(24)
#define NODE_HALFSIZE		(NODE_SIZE/2)

#define PADDING_SIZE			24

#include <stdio.h>
#include <math.h>

QString formatBytesToSize(float bytes);

int treeHeight = 0;
int fragmentsY = 0;
int fragmentsHeight = 30;// FIXED
bool showTextTreeNode = false;
QColor usedColor(153, 217, 234);
QColor emptyColor(230, 230, 230);

double				zoom = 1;

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
static void calcNodePosition(BuddyNode* node, int x, int y, double width)
{
	int half = width/2;

	node->y = y;
	node->x = x + (half-NODE_HALFSIZE);

	if(node->isSplitted())
	{
		int childrenY = y + NODE_SIZE+20;
		calcNodePosition(node->getLeft(), x, childrenY, half);
		calcNodePosition(node->getRight(), x+half, childrenY, half);
	}
	treeHeight = qMax(y+NODE_SIZE, treeHeight);
}

static void paintNodeTree(QPainter& p, BuddyNode* parentNode, BuddyNode* node)
{

	if(!node)
		return;

	if(parentNode != NULL)
	{
		p.setPen(Qt::black);
		p.drawLine(parentNode->x + NODE_HALFSIZE, parentNode->y + NODE_HALFSIZE, node->x + NODE_HALFSIZE, node->y + NODE_HALFSIZE);
	}
	paintNodeTree(p, node, node->getLeft());
	paintNodeTree(p, node, node->getRight());

	if(node->isSplitted())
	{
		if(showTextTreeNode)
		{
			p.setBrush(Qt::white);
		}
		else
		{
			p.setPen(QColor(250,250,250));
			p.setBrush(Qt::white);
			p.drawEllipse(node->x, node->y, NODE_SIZE, NODE_SIZE);
			p.setBrush(Qt::FDiagPattern);
		}
	}
	else
	{
		if(!node->isUsed())
		{
			p.setBrush(emptyColor);
		}
		else
		{
			if(!node->highlight)
				p.setBrush(usedColor);
			else
				p.setBrush(QColor(255, 245, 64));
		}
		int lx = node->x+NODE_HALFSIZE;
		int sy = node->y;
		int ey = fragmentsY-4;
		p.drawLine(lx, sy, lx, ey);

		QPainterPath path;
		path.moveTo(lx, ey);
		path.lineTo(lx+4, ey-6);
		path.lineTo(lx-4, ey-6);
		path.lineTo(lx, ey);
		p.fillPath(path, Qt::black);
	}
	p.setPen(Qt::black);
	p.drawEllipse(node->x, node->y, NODE_SIZE, NODE_SIZE);

	if(showTextTreeNode)
	{
		p.setPen(Qt::black);
		QFontMetrics fm(p.font());
		QString text = formatBytesToSize(node->getSize());
		int     textWidth = fm.width(text);
		int     textHeight = fm.height();

		p.drawText(
				node->x + (NODE_SIZE-textWidth)/2,
				node->y + (NODE_SIZE-textHeight)/2+fm.ascent(), text);
	}
}
static void paintNodeFragment(QPainter& p, BuddyNode* node, double x, double y, double width, int height)
{
	if(!node)
		return;

	double half = width/2;

	paintNodeFragment(p, node->getLeft(),  x     , y, half, height);
	paintNodeFragment(p, node->getRight(), x+half, y, half, height);

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

BuddySystemCanvas::BuddySystemCanvas(BuddySystem* sys, QWidget* parent) : QAbstractScrollArea(parent), system(sys)
{
	maxWidth = NODE_SIZE;
	maxHeight = NODE_SIZE;
	needRecompute = true;
	mousePressed = false;

	//
	// Inicializar barras
	//
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}
BuddySystemCanvas::~BuddySystemCanvas()
{
}
void BuddySystemCanvas::rebuild()
{
	maxWidth = (viewport()->width() - PADDING_SIZE*2)*zoom;
	treeHeight = 0;
	calcNodePosition(system->getRoot(), 0, 0, maxWidth);
	fragmentsY = treeHeight + 50;
	maxHeight = fragmentsY+fragmentsHeight;

	viewport()->update();

	setup();
}
void BuddySystemCanvas::paintEvent(QPaintEvent* e)
{
	QPainter p(this->viewport());


	int vw = width();
	int vh = height();

	p.fillRect(0, 0, vw, vh, Qt::white);

	int sx = scrollOffset(horizontalScrollBar(), maxWidth, vw);
	int sy = scrollOffset(verticalScrollBar()  , maxHeight, vh);

	p.translate(sx, sy);

	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::TextAntialiasing, true);


	QFont f = p.font();
	f.setPixelSize(9);;
	p.setFont(f);

	paintNodeTree(p, NULL, system->getRoot());

	p.setBrush(Qt::NoBrush);
	paintNodeFragment(p, system->getRoot(), 0, fragmentsY, maxWidth, fragmentsHeight);
}
void BuddySystemCanvas::resizeEvent(QResizeEvent* e)
{
	//setup();
	rebuild();
}
void BuddySystemCanvas::wheelEvent(QWheelEvent* e)
{
	if(e->modifiers() & Qt::ControlModifier)
	{
		if(e->delta() < 0) zoom -= 0.2;
		else if(e->delta() > 0) zoom += 0.2;

		if(zoom < 1) zoom = 1;
		else if(zoom > 100) zoom = 100;

		rebuild();
	}
	else
	{
		QScrollBar* sbr;

		if(e->modifiers() & Qt::ShiftModifier)
			sbr = horizontalScrollBar();
		else
			sbr = verticalScrollBar();

		if(e->delta() < 0)
			sbr->setValue(sbr->value() + sbr->singleStep());
		else if(e->delta() > 0)
			sbr->setValue(sbr->value() - sbr->singleStep());
	}
}
void BuddySystemCanvas::keyPressEvent(QKeyEvent* e){}
void BuddySystemCanvas::keyReleaseEvent(QKeyEvent* e){}
void BuddySystemCanvas::mouseMoveEvent(QMouseEvent* e)
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
void BuddySystemCanvas::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void BuddySystemCanvas::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}

void BuddySystemCanvas::setup()
{

	//
	// Actualizar barras de desplazamiento
	//
	updateScrollBar(horizontalScrollBar(), maxWidth+(PADDING_SIZE*2)-1, viewport()->width());
	updateScrollBar(verticalScrollBar(), maxHeight +(PADDING_SIZE*2)-1, viewport()->height());
}
