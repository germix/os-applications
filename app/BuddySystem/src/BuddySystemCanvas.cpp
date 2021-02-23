#include "BuddySystemCanvas.h"

#include "BuddyNode.h"
#include "BuddySystem.h"

#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>

#define NODE_SIZE			(48)
#define NODE_HALFSIZE		(NODE_SIZE/2)
#define BORDER_SIZE			30

#include <stdio.h>
#include <math.h>

QString formatBytesToSize(float bytes);

static void updateScrollBar(QScrollBar* bar, int max, int page)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(4);
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
	maxWidth = 0;
	maxHeight = 0;
	calcNodePosition(system->getRoot(), 0, 0);
	viewport()->update();

	setup();
}
void BuddySystemCanvas::paintEvent(QPaintEvent* e)
{
	QPainter p(this->viewport());

	p.fillRect(QRect(0, 0, width(), height()), Qt::white);

	int vw = width();
	int vh = height();
	int xScroll = BORDER_SIZE - horizontalScrollBar()->value();
	int yScroll = BORDER_SIZE - verticalScrollBar()->value();

	p.translate(
			xScroll + ((vw<maxWidth)?0:(vw/2 - maxWidth/2 - BORDER_SIZE)),
			yScroll + ((vh<maxHeight)?0:(vh/2 - maxHeight/2 - BORDER_SIZE)));

	p.setRenderHint(QPainter::Antialiasing, true);
	p.setRenderHint(QPainter::TextAntialiasing, true);
	paintNode(p, NULL, system->getRoot());
}
void BuddySystemCanvas::resizeEvent(QResizeEvent* e)
{
	setup();
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
	updateScrollBar(horizontalScrollBar(), maxWidth+BORDER_SIZE*2, viewport()->width());
	updateScrollBar(verticalScrollBar(), maxHeight+BORDER_SIZE*2, viewport()->height());
}
void BuddySystemCanvas::paintNode(QPainter& p, BuddyNode* parentNode, BuddyNode* node)
{
	QColor usedColor(153, 217, 234);
	QColor emptyColor(230, 230, 230);

	if(!node)
		return;

	if(parentNode != NULL)
	{
		p.setPen(Qt::black);
		p.drawLine(parentNode->x + NODE_HALFSIZE, parentNode->y + NODE_HALFSIZE, node->x + NODE_HALFSIZE, node->y + NODE_HALFSIZE);
	}
	paintNode(p, node, node->getLeft());
	paintNode(p, node, node->getRight());

	if(node->isSplitted())
	{
		/*
		p.setPen(QColor(250,250,250));
		p.setBrush(Qt::white);
		p.drawEllipse(node->x, node->y, NODE_SIZE, NODE_SIZE);
		p.setBrush(Qt::FDiagPattern);
		*/
		p.setBrush(Qt::white);
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
	}
	p.setPen(Qt::black);
	p.drawEllipse(node->x, node->y, NODE_SIZE, NODE_SIZE);

	p.setPen(Qt::black);

	QFontMetrics fm(p.font());
	QString text = formatBytesToSize(node->getSize());
	int     textWidth = fm.width(text);
	int     textHeight = fm.height();

	p.drawText(
			node->x + (NODE_SIZE-textWidth)/2,
			node->y + (NODE_SIZE-textHeight)/2+fm.ascent(), text);

}
int BuddySystemCanvas::calcNodePosition(BuddyNode* node, int x, int y)
{
	if(!node->isSplitted())
	{
		node->x = x;
		node->y = y;
		x += NODE_SIZE;
	}
	else
	{
		int childrenY = y + NODE_SIZE+20;
		int childrenRight = x;

		childrenRight = calcNodePosition(node->getLeft(), childrenRight, childrenY);
		childrenRight+= 20;
		childrenRight = calcNodePosition(node->getRight(), childrenRight, childrenY);

		int childrenX1 = node->getLeft()->x;
		int childrenX2 = node->getRight()->x+NODE_SIZE;
		int childrenWidth = childrenX2-childrenX1;

		node->x = childrenX1 + (childrenWidth/2 - NODE_SIZE/2);
		node->y = y;

		x = childrenRight;
	}
	maxWidth = qMax(x, maxWidth);
	maxHeight = qMax(y+NODE_SIZE, maxHeight);
	return x;
}
