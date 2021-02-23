#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H
#include <QStyledItemDelegate>

class MyItemDelegate : public QStyledItemDelegate
{
	QWidget* widget;
public:
	MyItemDelegate(QWidget* w);
	~MyItemDelegate();
public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
private:
	void paintText(QPainter* p, const QModelIndex& index, QStyle* style, QStyleOptionViewItem* vopt) const;
	void paintFocus(QPainter* p, QStyle* style, QStyleOptionViewItem* vopt) const;
};

#endif // MYITEMDELEGATE_H
