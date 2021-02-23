#ifndef WORKINGSETCANVAS_H
#define WORKINGSETCANVAS_H
#include <QVector>
#include <QAbstractScrollArea>

#include "WorkingSet.h"

class WorkingSetCanvas : public QAbstractScrollArea
{
    Q_OBJECT

	QVector<PageWS *>	wset;
	int					wsize;
	QVector<int>		sequence;

	int					tail;
	int					current;

	QFont				font;
	int					fontHeight;
	int					sequenceWidth;

	QPoint				refPos;
	bool				mousePressed;
public:
	explicit WorkingSetCanvas(QWidget *parent = 0);
	~WorkingSetCanvas();
signals:

public slots:
public:
	void paintEvent(QPaintEvent* e);
	void resizeEvent(QResizeEvent* e);
	void wheelEvent(QWheelEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
public:
	void clear();
	void init(const QVector<int>& seq, int wsize);
	void step(const QVector<PageWS *> &wset, int tail, int current);
private:
	void updateScrollBars();
};

#endif // WORKINGSETCANVAS_H
