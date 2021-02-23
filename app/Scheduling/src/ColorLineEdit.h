#ifndef COLORLINEEDIT_H
#define COLORLINEEDIT_H
#include <QLineEdit>

#include "utils.h"

class ColorLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit ColorLineEdit(QWidget *parent = 0);
	~ColorLineEdit();
signals:

public:
	void setText(const QString& text);
public slots:
	void onTextEdited(const QString& text);
};

#endif // COLORLINEEDIT_H
