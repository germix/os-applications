#ifndef BUDDYSYSTEMCANVAS2_H
#define BUDDYSYSTEMCANVAS2_H
#include <QAbstractScrollArea>

class BuddyNode;
class BuddySystem;

class BuddySystemCanvas2 : public QAbstractScrollArea
{
	Q_OBJECT
	BuddySystem*		system;
	QPoint				translate;

	double				zoom;

	QPoint				refPos;
	bool				mousePressed;

	double				boxWidth;
	double				boxHeight;
public:
	BuddySystemCanvas2(BuddySystem* sys, QWidget* parent = 0);
	~BuddySystemCanvas2();
public:
	void rebuild();
public:
	virtual void paintEvent(QPaintEvent* e);
	virtual void resizeEvent(QResizeEvent* e);

	virtual void keyPressEvent(QKeyEvent* e);
	virtual void keyReleaseEvent(QKeyEvent* e);

	virtual void wheelEvent(QWheelEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
private:
	void updateScrollBars();
};

#endif // BUDDYSYSTEMCANVAS2_H
