#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QtGui>


AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

	// Mostrat información
	{
		QString s;
#if 1
#else
		QFileInfo info(qApp->applicationFilePath());
#endif
		s += tr("Hora: ");
#if 1
		s += __TIME__;
#else
		s += info.lastModified().time().toString();
#endif
		s += "\n";
		s += tr("Fecha: ");
#if 1
		s += __DATE__;
#else
		s += info.lastModified().date().toString();
#endif
		s += "\n";
		s += tr("Programador: Germán Martí­nez");
		ui->label->setText(s);
	}
	// Ajustar tamaño
	{
		QSize s = sizeHint();
		setMinimumSize(s);
		setMaximumSize(s);
	}
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
