#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QMessageBox>

#include "MyItemModel.h"
#include "MyItemDelegate.h"
#include "GantCanvas.h"
#include "HelpDialog.h"
#include "AboutDialog.h"

#include "Process.h"
#include "TimeMarkManager.h"
#include "SchedulingFIFO.h"
#include "SchedulingSJF.h"
#include "SchedulingSRT.h"
#include "SchedulingRR.h"

#include <QSplitter>

#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//
	// Crear tabla
	//
	QHeaderView* hv;

	model = createModel(2);
	table = ui->tableView;
	table->setModel(model);
	table->setItemDelegate(new MyItemDelegate(table));

	hv = table->horizontalHeader();
#if (QT_VERSION >= 0x050000)
	hv->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	hv->setSectionResizeMode(1, QHeaderView::Stretch);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
	hv->setResizeMode(0, QHeaderView::ResizeToContents);
	hv->setResizeMode(1, QHeaderView::Stretch);
	table->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
	//setCentralWidget(table);

	//
	// Crear vista para el gráfico Gant
	//
	gantCanvas = new GantCanvas();

	//
	// Cargar algoritmos
	//
	algorithms.append(new SchedulingFIFO());
	algorithms.append(new SchedulingSJF());
	algorithms.append(new SchedulingSRT());
	algorithms.append(new SchedulingRR());
	for(int i = 0; i < algorithms.size(); i++)
	{
		ui->cboAlgoritms->addItem(algorithms.at(i)->name());
	}
	QWidget* spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->mainToolBar->addWidget(spacer);
	ui->mainToolBar->addWidget(ui->lblAlgoritms);
	ui->mainToolBar->addWidget(ui->cboAlgoritms);
	ui->mainToolBar->addAction(ui->actionSimulatorRun);

#if 0
	{
		QWidget* central = new QWidget();
		QWidget* oldCentral = centralWidget();
		QVBoxLayout* layout = new QVBoxLayout(central);

		layout->addWidget(table);
		layout->addWidget(gantCanvas);
		layout->setContentsMargins(0, 0, 0, 0);

		central->setLayout(layout);

		setCentralWidget(central);
		delete oldCentral;
	}
#else
	{
		QSplitter* splitter = new QSplitter(Qt::Vertical);
		QWidget*   oldCentral = centralWidget();

		splitter->addWidget(table);
		splitter->addWidget(gantCanvas);

		setCentralWidget(splitter);
		delete oldCentral;
	}
#endif
	updateInitialIndexButton();
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::slotFileNew()
{
	MyItemModel* mdl = new MyItemModel(model ? model->isEnumFromZero() : false);
	model = mdl;
	table->setModel(mdl);
	gantCanvas->clear();
	ui->statusBar->showMessage("");
	updateInitialIndexButton();
}

void MainWindow::slotFileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QString(), tr("Archivos de planificación (*.xml)"));
	if(!fileName.isEmpty())
	{
		MyItemModel* mdl = new MyItemModel();

		if(!mdl->open(fileName))
		{
			delete mdl;
		}
		else
		{
			model = mdl;
			table->setModel(mdl);
			gantCanvas->clear();
			gantCanvas->setRulerStep(mdl->getRulerSteps());
			ui->statusBar->showMessage(mdl->getDescription());
			updateInitialIndexButton();
		}
	}
}
void MainWindow::slotFileSave()
{
	if(!model->hasFilename())
		slotFileSaveAs();
	else
		model->save(model->getFilename());
}
void MainWindow::slotFileSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), QString(), tr("Archivos de planificación (*.xml)"));
	if(!fileName.isEmpty())
	{
		model->save(fileName);
	}
}
void MainWindow::slotFileExit()
{
	close();
}
void MainWindow::slotHelpHelp()
{
	HelpDialog dlg;
	dlg.exec();
}
void MainWindow::slotHelpAbout()
{
	AboutDialog dlg;
	dlg.exec();
}
void MainWindow::slotSimulatorExec()
{
	int idx;

	if(-1 != (idx = ui->cboAlgoritms->currentIndex()))
	{
		MyItemModel* model = (MyItemModel*)table->model();
		QVector<Process*> processList;

		//
		// Generar lista de procesos
		//
		for(int r = 0; r < model->rowCount(); r++)
		{
			QString at = model->data(model->index(r, 0)).toString();
			QString ef = model->data(model->index(r, 1)).toString();

			processList.append(new Process(r, at.toInt(), ef));
		}

		//
		// Ejecutar algoritmo
		//
		TimeMarkManager marks(processList.size());
		int maxTime = algorithms.at(idx)->exec(processList, marks);
		if(maxTime >= 0)
		{
			gantCanvas->setLines(marks.toGantLines(), maxTime);
		}
		//
		// Eliminar lista de procesos
		//
		for(int r = 0; r < processList.size(); r++)
		{
			delete processList.at(r);
		}
		processList.clear();
	}
}
void MainWindow::slotSimulatorAddProcess()
{
	model->addProcess();
}
void MainWindow::slotSimulatorRemoveProcess()
{
	QList<int> rows;
	{
		QSet<int> set;
#if 1
		QModelIndexList selection = table->selectionModel()->selectedRows();
#else
		QModelIndexList selection = table->selectionModel()->selectedIndexes();
#endif
		for(int i = 0; i < selection.size(); i++)
		{
			set.insert(selection.at(i).row());
		}
		rows = set.values();
		qSort(rows);
	}
	if(!rows.isEmpty())
	{
		QString msg;

		if(rows.size() == 1)
			msg += tr("¿Desea eliminar el proceso seleccionado?");
		else
			//msg += tr("¿Desea eliminar los procesos seleccionados?");
			msg += QString(tr("¿Desea eliminar los %1 procesos seleccionados?")).arg(rows.size());


		if(QMessageBox::Ok == QMessageBox::question(this, tr("Eliminar procesos"), msg, QMessageBox::Ok, QMessageBox::Cancel))
		{
			for(int i = rows.size()-1; i >= 0; i--)
			{
				model->removeProcess(rows.at(i));
			}
		}
	}
}
void MainWindow::slotSimulatorEditMarks()
{
	QString step = QInputDialog::getText(
				this,
				tr("Distancia entre marcas"),
				tr("Distancia entre marcas:"),
				QLineEdit::Normal,
				QString::number(model->getRulerSteps()));

	if(!step.isEmpty())
	{
		model->setRulerSteps(step.toInt());
		gantCanvas->setRulerStep(step.toInt());
	}
}
void MainWindow::slotSimulatorEditDescription()
{
	QString descr = QInputDialog::getText(
				this,
				tr("Descripción"),
				tr("Descripción:"),
				QLineEdit::Normal,
				model->getDescription());

	if(!descr.isEmpty())
	{
		model->setDescripcion(descr);
		ui->statusBar->showMessage(descr);
	}
}
void MainWindow::slotSimulatorChangeInitialIndex()
{
	model->setInitialIndex(!model->getInitialIndex());
	updateInitialIndexButton();
}
void MainWindow::updateInitialIndexButton()
{
	if(model->getInitialIndex() == 0)
		ui->actionSimulatorInitialIndex->setIcon(QIcon(":/view-initial-index-0.png"));
	else
		ui->actionSimulatorInitialIndex->setIcon(QIcon(":/view-initial-index-1.png"));
}

MyItemModel* MainWindow::createModel(int process)
{
	MyItemModel* model = new MyItemModel(false);

#if 1
	model->addProcess(0, "10,5,80,55,10");
	model->addProcess(0, "25,10,40,55,40,30,25");
	model->addProcess(0, "15,100,10,20,5");
#else
	model->addProcess(0, "9");
	model->addProcess(2, "5");
	model->addProcess(3, "2");
	model->addProcess(5, "4");
#endif
/*
	model->addProcess(0, "4,5,3");
	model->addProcess(0, "5,4");
	model->addProcess(0, "3,5");
*/
	return model;
}


