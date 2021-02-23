#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QAbstractScrollArea>

class TableElement;

class TableView : public QAbstractScrollArea
{
	Q_OBJECT

	int							cols;
	int							rows;
	QVector<TableElement*>		elems;
	int							elemWidth;
	int							elemHeight;
	int							tableWidth;
	int							tableHeight;


	QPoint						refPos;
	bool						mousePressed;
public:
	explicit TableView(QWidget *parent = 0);
	~TableView();
signals:

public slots:
	void slotScrollValueChanged(int value);
public:
	void setElements(int cols, int rows, const QVector<TableElement*>& elems);
public:
	void paintEvent(QPaintEvent* e);
	void resizeEvent(QResizeEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
private:
	void updateScrollBars();
	void updateScrollBar(QScrollBar* bar, int max, int page);
};

#endif // TABLEVIEW_H
