#ifndef BUDDYSYSTEMCANVAS_H
#define BUDDYSYSTEMCANVAS_H
#include <QAbstractScrollArea>

class BuddyNode;
class BuddySystem;

class BuddySystemCanvas : public QAbstractScrollArea
{
	Q_OBJECT
	BuddySystem*		system;

	bool				needRecompute;

	int					maxWidth;
	int					maxHeight;

	QPoint				refPos;
	bool				mousePressed;
public:
	BuddySystemCanvas(BuddySystem* sys, QWidget* parent = 0);
	~BuddySystemCanvas();
public:
	void rebuild();
public:
	virtual void paintEvent(QPaintEvent* e);
	virtual void resizeEvent(QResizeEvent* e);

	virtual void keyPressEvent(QKeyEvent* e);
	virtual void keyReleaseEvent(QKeyEvent* e);

	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
private:
	void setup();
	void paintNode(QPainter& p, BuddyNode* parentNode, BuddyNode* node);
	int calcNodePosition(BuddyNode* node, int x, int y);
};

#endif // BUDDYSYSTEMCANVAS_H
