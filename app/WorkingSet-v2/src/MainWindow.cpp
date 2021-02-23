#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QVector>
#include <QMessageBox>

#include "WorkingSet.h"
#include "AboutDialog.h"

#include "WorkingSetCanvas.h"

class WorkingSetListenerImpl : public WorkingSetListener
{
	QListWidget* list;
	WorkingSetCanvas* canvas;
public:
	WorkingSetListenerImpl(QListWidget* l, WorkingSetCanvas* c) : list(l), canvas(c)
	{
	}
	virtual void init()
	{
		list->clear();
	}
	virtual void step(const QVector<PageWS*>& wset, int tail, int head)
	{
		QString txt;

		txt = QString().sprintf("%2d - {", (head+1));
		for(int i = 0; i < wset.size(); i++)
		{
			if(i == 0)
				txt += QString::number(wset.at(i)->numPage);
			else
				txt += "," + QString::number(wset.at(i)->numPage);
		}
		txt += "}";

		list->addItem(new QListWidgetItem(txt));

		canvas->step(wset, tail, head);
	}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	qApp->installEventFilter(this);

	connect(ui->btnRun, SIGNAL(clicked()), this, SLOT(slotRun()));
	connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slotRun()));
	connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));

	ui->lineEdit->setFocus();

	canvas = new WorkingSetCanvas();
	ui->gridLayout->addWidget(canvas, 1, 0, 3, 1);

	invalid = true;
	wsetListener = new WorkingSetListenerImpl(ui->listWidget, canvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::slotRun()
{
	QVector<int> seq = getSequence();
	int          wsize = getWindowSize();

	if(!seq.isEmpty() && wsize > 0)
	{
		if(invalid)
		{
			wsetModel.init(wsetListener, seq, wsize);
			canvas->init(seq, wsize);
			invalid = false;
			wsetModel.step();

			QVector<PageWS*> pages = wsetModel.getPages();

			treeItems.clear();
			for(int i = 0; i < pages.size(); i++)
			{
				PageWS* p = pages.at(i);
				treeItems.append(
						new QTreeWidgetItem(ui->treeWidget,
						QStringList() << QString::number(p->numPage) << QString::number(p->ref)));
			}

			ui->spinBox->setEnabled(false);
			ui->lineEdit->setEnabled(false);
		}
		else
		{
			wsetModel.step();
			{
				QVector<PageWS*> pages = wsetModel.getPages();
				for(int i = 0; i < pages.size(); i++)
				{
					PageWS* p = pages.at(i);
					QTreeWidgetItem* item = treeItems.at(i);

					item->setText(1, QString::number(p->ref));
				}
			}
			if(wsetModel.atEnd())
			{
				ui->btnRun->setEnabled(false);
				//QMessageBox::information(this, NULL, "Fin de la secuencia", QMessageBox::Ok);
			}
		}
		ui->btnCancel->setEnabled(true);
		ui->statusBar->showMessage("Fallos de páginas = " + QString::number(wsetModel.getPageFaults()));
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
void MainWindow::slotCancel()
{
	invalid = true;
	ui->btnRun->setEnabled(true);
	ui->spinBox->setEnabled(true);
	ui->lineEdit->setEnabled(true);
	ui->listWidget->clear();
	ui->treeWidget->clear();
	ui->statusBar->clearMessage();
	ui->btnCancel->setEnabled(false);
	canvas->clear();
	treeItems.clear();
}
void MainWindow::slotTextChanged(const QString& text)
{
	invalid = true;
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
				ui->lineEdit->setFocus();
				return QVector<int>();
			}
		}
	}
	return iseq;
}
int MainWindow::getWindowSize()
{
	return ui->spinBox->value();
}
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	return (event->type() == QEvent::StatusTip);
}
