#ifndef CANVAS_H
#define CANVAS_H
#include <QAbstractScrollArea>

class MemoryManager;

class Canvas : public QAbstractScrollArea
{
    Q_OBJECT
	MemoryManager*		mmgr;
	double				zoom;
	QPoint				refPos;
	bool				mousePressed;
	double				maxWidth;
public:
	explicit Canvas(MemoryManager* mmgr, QWidget *parent = 0);
	~Canvas();
signals:

public slots:
private:
	void setup();
public:
	void reset();
public:
	virtual void paintEvent(QPaintEvent* e);
	virtual void wheelEvent(QWheelEvent* e);
	virtual void resizeEvent(QResizeEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
};

#endif // CANVAS_H
