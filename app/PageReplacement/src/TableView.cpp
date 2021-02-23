#include "TableView.h"
#include <QPainter>
#include <QFont>
#include <QFontMetrics>

#include "TableElement.h"

#include <QScrollBar>
#include <QMouseEvent>

#define PADDING_SIZE		10

static int scrollOffset(QScrollBar* bar, int tableSize, int viewportSize)
{
	if((viewportSize-PADDING_SIZE*2)>tableSize)
	{
		return (viewportSize/2 - tableSize/2);
	}
	return -bar->value() + PADDING_SIZE;
}

TableView::TableView(QWidget *parent) : QAbstractScrollArea(parent)
{
	cols = 0;
	rows = 0;
	elemWidth = 32;
	elemHeight = 32;
	tableWidth = 0;
	tableHeight = 0;
	mousePressed = false;

	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

#if (QT_VERSION >= 0x050000)
	connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &slotScrollValueChanged);
	connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &slotScrollValueChanged);
#else
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollValueChanged(int)));
	connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollValueChanged(int)));
#endif
}
TableView::~TableView()
{
#if (QT_VERSION >= 0x050000)
	disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &slotScrollValueChanged);
	disconnect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &slotScrollValueChanged);
#else
	disconnect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollValueChanged(int)));
	disconnect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(slotScrollValueChanged(int)));
#endif
}
void TableView::paintEvent(QPaintEvent* e)
{
	QPainter p(viewport());

	p.fillRect(rect(), Qt::white);

	int vw = width();
	int vh = height();
	int sx = scrollOffset(horizontalScrollBar(), tableWidth, vw);
	int sy = scrollOffset(verticalScrollBar()  , tableHeight, vh);

	p.translate(sx, sy);

	if(!elems.isEmpty())
	{
		QFont font("Courier", 16);
		QFontMetrics fm(font);
		QFont font2("Courier", 8);
		QFontMetrics fm2(font2);

		p.setFont(font);
		for(int x = 0; x < cols; x++)
		{
			for(int y = 0; y < rows; y++)
			{
				int idx = (y*cols)+x;
				int elemX = x*elemWidth;
				int elemY = y*elemHeight;
				TableElement* elem = elems.at(idx);

				if(elem != NULL)
				{
					//elem->paint(p, fm, elemX, elemY, elemWidth, elemHeight);
					elem->paint(p, font, fm, font2, fm2, elemX, elemY, elemWidth, elemHeight);
				}
				p.setPen(Qt::black);
				p.drawRect(elemX, elemY, elemWidth, elemHeight);
			}
		}
	}
}
void TableView::resizeEvent(QResizeEvent* e)
{
	updateScrollBars();
}
void TableView::mouseMoveEvent(QMouseEvent* e)
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
void TableView::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		refPos = e->pos();
		mousePressed = true;
		setCursor(Qt::SizeAllCursor);
	}
}
void TableView::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
		mousePressed = false;
		setCursor(Qt::ArrowCursor);
	}
}
void TableView::setElements(int cols, int rows, const QVector<TableElement*>& elems)
{
	this->cols = cols;
	this->rows = rows;
	this->elems = elems;
	this->tableWidth = (cols*elemWidth);
	this->tableHeight = (rows*elemHeight);
	updateScrollBars();
	viewport()->repaint();
}
void TableView::updateScrollBars()
{
	updateScrollBar(horizontalScrollBar(), tableWidth+(PADDING_SIZE*2), viewport()->width());
	updateScrollBar(verticalScrollBar(),   tableHeight+(PADDING_SIZE*2), viewport()->height());
}
void TableView::updateScrollBar(QScrollBar* bar, int max, int page)
{
	bar->setMaximum(qMax(0, max-page+1));
	bar->setPageStep(page);
	bar->setSingleStep(4);
}
void TableView::slotScrollValueChanged(int value)
{

}

