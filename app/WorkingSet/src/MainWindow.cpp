#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVector>
#include <QMessageBox>

#include "WorkingSet.h"
#include "AboutDialog.h"

class WorkingSetListenerImpl : public WorkingSetListener
{
	QListWidget* list;
public:
	WorkingSetListenerImpl(QListWidget* l) : list(l)
	{
	}
	virtual void init()
	{
		list->clear();
	}
	virtual void step(int num, const QVector<int>& wset)
	{
		QString txt;

		txt = QString().sprintf("%2d - {", num);
		for(int i = 0; i < wset.size(); i++)
		{
			if(i == 0)
				txt += QString::number(wset.at(i));
			else
				txt += "," + QString::number(wset.at(i));
		}
		txt += "}";

		list->addItem(new QListWidgetItem(txt));
	}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	qApp->installEventFilter(this);

	wsetListener = new WorkingSetListenerImpl(ui->listWidget);

	connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(slotRun()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slotRun()));
	connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
	connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
	ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	return (event->type() == QEvent::StatusTip);
}
void MainWindow::slotRun()
{
	QVector<int> seq = getSequence();
	int          wsize = getWindowSize();

	if(!seq.isEmpty() && wsize > 0)
	{
		int pf = wset.run(wsetListener, wsize, seq);

		ui->statusBar->showMessage("Fallos de páginas = " + QString::number(pf));
	}
}
void MainWindow::slotFileExit()
{
	close();
}
void MainWindow::slotHelpAbout()
{
	AboutDialog dlg;
	dlg.exec();
}
void MainWindow::slotTextChanged(const QString& text)
{
	ui->listWidget->clear();
	ui->statusBar->clearMessage();
}
void MainWindow::slotValueChanged(int value)
{
	ui->listWidget->clear();
	ui->statusBar->clearMessage();
}
QVector<int> MainWindow::getSequence()
{
	QString txt = ui->lineEdit->text();
	QVector<int> iseq;

	if(!txt.isEmpty())
	{
		QStringList seq = txt.split(",");

		for(int i = 0; i < seq.length(); i++)
		{
			bool ok;
			QString str = seq.at(i);
			int num = str.toInt(&ok);
			if(ok)
			{
				iseq.append(num);
			}
			else
			{
				QMessageBox::information(this, NULL, "Cadena de referencias mal formada", QMessageBox::Ok);
			}
		}
	}
	return iseq;
}
int MainWindow::getWindowSize()
{
	return ui->spinBox->value();
}

