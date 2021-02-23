#ifndef GANTCANVAS_H
#define GANTCANVAS_H
#include <QAbstractScrollArea>

class GantLine;
class GantSegment;

class GantCanvas : public QAbstractScrollArea
{
	int					zoom;
	int					rulerStep;

	QVector<GantLine*>	lines;

	int					offsetX;
	int					maxTime;

	QFont				font;
	int					fontHeight;
	int					lineHeight;

	int					gantWidth;
	int					gantHeight;

	QPoint				refPos;
	bool				mousePressed;
public:
	explicit GantCanvas(QWidget* parent = 0);
	explicit GantCanvas(const QVector<GantLine*>& lines, int maxTime, QWidget* parent = 0);
	~GantCanvas();
public:
	void clear();
	void setLines(const QVector<GantLine*>& lines, int maxTime);
	void setRulerStep(int step);
public:
	virtual void paintEvent(QPaintEvent* e);
	virtual void resizeEvent(QResizeEvent* e);
	virtual void wheelEvent(QWheelEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
private:
	void init();
	void setup();
	void deleteLines();
	void drawRuler(QPainter& p, int x, int y1, int y2, int max);
	void drawSegments(QPainter& p, int x, int y, const QVector<GantSegment*>& segments);
};

#endif // GANTCANVAS_H
