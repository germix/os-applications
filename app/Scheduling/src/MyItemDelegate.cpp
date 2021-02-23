#include "MyItemDelegate.h"
#include <QPainter>
#include <QApplication>

#include "utils.h"
#include "ColorLineEdit.h"

#include <QTextLayout>

static QSizeF viewItemTextLayout(QTextLayout &textLayout, int lineWidth)
{
	qreal height = 0;
	qreal widthUsed = 0;
	textLayout.beginLayout();
	while(true)
	{
		QTextLine line = textLayout.createLine();
		if (!line.isValid())
			break;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
	}
	textLayout.endLayout();
	return QSizeF(widthUsed, height);
}


MyItemDelegate::MyItemDelegate(QWidget* w) : widget(w)
{
}
MyItemDelegate::~MyItemDelegate()
{
}
void MyItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if(index.column() == 1)
	{
		QStyle* style = (widget ? widget->style() : qApp->style());
		QStyleOptionViewItem opt = option;
		initStyleOption(&opt, index);

		// Guardar contexto
		painter->save();

		// Pintar el fondo
		style->proxy()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

		// Pintar el texto
		paintText(painter, index, style, &opt);

		// Pinter la marca del foco
		paintFocus(painter, style, &opt);

		// Restaurar contexto
		painter->restore();

		return;
	}
	QStyledItemDelegate::paint(painter, option, index);
}
QWidget* MyItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if(index.column() == 1)
	{
		return new ColorLineEdit(parent);
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}
void MyItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if(index.column() == 1)
	{
		((ColorLineEdit*)editor)->setText(index.data(Qt::DisplayRole).toString());
		return;
	}
	QStyledItemDelegate::setEditorData(editor, index);
}
void MyItemDelegate::paintText(QPainter* p, const QModelIndex& index, QStyle* style, QStyleOptionViewItem* vopt) const
{
#if (QT_VERSION >= 0x050000)
	QString text = vopt->text;
#else
	QString text = index.data(Qt::DisplayRole).toString();
#endif
	QRect textRect = vopt->rect;
	//QRect textRect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemText, vopt, widget);
	QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
	if(cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
	{
		cg = QPalette::Inactive;
	}
	if(vopt->state & QStyle::State_Editing)
	{
		p->setPen(vopt->palette.color(cg, QPalette::Text));
		p->drawRect(textRect.adjusted(0, 0, -1, -1));
	}
	if(vopt->state & QStyle::State_Selected)
	{
		p->fillRect(textRect, vopt->palette.color(cg, QPalette::Highlight));
	}
	if(!text.isEmpty())
	{
#if (QT_VERSION >= 0x050000)
		const QWidget *widget = vopt->widget;
		const int textMargin = style->proxy()->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;
#else
		const int textMargin = style->proxy()->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, widget) + 1;
#endif
#if 0
		QRect textRect = rect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
#else
		textRect = textRect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
#endif
#if (QT_VERSION >= 0x050000)
		const bool wrapText = vopt->features & QStyleOptionViewItem::WrapText;
#else
		const bool wrapText = true;
#endif
		QTextOption textOption;
		textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
		textOption.setTextDirection(vopt->direction);
		textOption.setAlignment(QStyle::visualAlignment(vopt->direction, vopt->displayAlignment));
		QTextLayout textLayout(text, vopt->font);
		textLayout.setTextOption(textOption);

		viewItemTextLayout(textLayout, textRect.width());

		qreal height = 0;
		qreal width = 0;
		const int lineCount = textLayout.lineCount();
		for(int j = 0; j < lineCount; ++j)
		{
			const QTextLine line = textLayout.lineAt(j);
			width = qMax<qreal>(width, line.width());
			height += line.height();
		}
		const QRect layoutRect = QStyle::alignedRect(vopt->direction, vopt->displayAlignment,
		QSize(int(width), int(height)), textRect);
		const QPointF position = layoutRect.topLeft();
#if 0
		QString elidedText;
		int elidedIndex = -1;
		for (int i = 0; i < lineCount; ++i)
		{
			const QTextLine line = textLayout.lineAt(i);
			if (i == elidedIndex)
			{
				qreal x = position.x() + line.x();
				qreal y = position.y() + line.y() + line.ascent();
				p->save();
				p->setFont(vopt->font);
				p->drawText(QPointF(x, y), elidedText);
				p->restore();
				break;
			}
			line.draw(p, position);
		}
#else
		if(lineCount >= 1)
		{
			const QTextLine line = textLayout.lineAt(0);
			qreal x = position.x() + line.x();
			qreal y = position.y() + line.y() + line.ascent();

			QVector<QColor> colors;

			p->setFont(vopt->font);
			fillColorsVector(text, colors);
			for(int i = 0; i < text.length(); i++)
			{
				QColor c = colors.at(i);
				QString s = text.at(i);

				p->setPen(c);
				p->drawText(QPointF(x, y), s);
				x += vopt->fontMetrics.width(s);
			}
		}
#endif
	}
}
void MyItemDelegate::paintFocus(QPainter* p, QStyle* style, QStyleOptionViewItem* vopt) const
{
	if(vopt->state & QStyle::State_HasFocus)
	{
		QStyleOptionFocusRect o;
		o.QStyleOption::operator=(*vopt);
		o.rect = style->proxy()->subElementRect(QStyle::SE_ItemViewItemFocusRect, vopt, widget);
		o.state |= QStyle::State_KeyboardFocusChange;
		o.state |= QStyle::State_Item;
		QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
		o.backgroundColor = vopt->palette.color(cg, (vopt->state & QStyle::State_Selected) ? QPalette::Highlight : QPalette::Window);
		style->proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, p, widget);
	}
}
