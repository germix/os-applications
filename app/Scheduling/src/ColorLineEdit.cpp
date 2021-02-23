#include "ColorLineEdit.h"
#include <QList>
#include <QEvent>
#include <QTextLayout>

ColorLineEdit::ColorLineEdit(QWidget *parent) : QLineEdit(parent)
{
#if (QT_VERSION >= 0x050000)
	connect(this, &ColorLineEdit::textEdited, this, &ColorLineEdit::onTextEdited);
#else
	connect(this, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));
#endif
}
ColorLineEdit::~ColorLineEdit()
{
#if (QT_VERSION >= 0x050000)
	disconnect(this, &ColorLineEdit::textEdited, this, &ColorLineEdit::onTextEdited);
#else
	disconnect(this, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));
#endif
}
void ColorLineEdit::setText(const QString& text)
{
	QLineEdit::setText(text);
	onTextEdited(text);
}
void ColorLineEdit::onTextEdited(const QString& text)
{
	QVector<QColor> colors;
	fillColorsVector(text, colors);

	int size = colors.size();
	QList<QInputMethodEvent::Attribute> attributes;

	attributes.reserve(size);
	for(int i = 0; i < size; i++)
	{
		if(colors[i].isValid())
		{
			QTextCharFormat tcf;
			tcf.setForeground(QBrush(colors[i]));
			attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, i-cursorPosition(), 1, tcf));
		}
	}
	QInputMethodEvent ime(QString(), attributes);
	inputMethodEvent(&ime);
}

/*
void ColorLineEdit::setCharColors(const QList<QColor> &colors)
{
	// See http://stackoverflow.com/questions/14417333/how-can-i-change-color-of-part-of-the-text-in-qlineedit.
	QList<QInputMethodEvent::Attribute> attributes;
	int size = colors.size();
	attributes.reserve(size);
	for (int ii = 0; ii < size ; ii++) {
		if (colors[ii].isValid()) {
			QTextCharFormat charFormat;
			charFormat.setForeground(QBrush(colors[ii]));
			const int start = ii - cursorPosition();
			const int length = 1;
			attributes.append(QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, start, length, charFormat));
		}
	}

	QInputMethodEvent ime(QString(), attributes);

	//QLineEdit::inputMethodEvent(&ime);
	inputMethodEvent(&ime);
//	QLineEdit::inputMethodEvent(&QInputMethodEvent(QString(), attributes));

	lastTextSize = text().size();
	this->colors = colors;
}

*/
