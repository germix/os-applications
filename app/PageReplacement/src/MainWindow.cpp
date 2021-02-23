#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLineEdit>

#include "PageReplacementAlgolFIFO.h"
#include "PageReplacementAlgolLRU.h"
#include "PageReplacementAlgolOPT.h"
#include "PageReplacementAlgolCLOCK.h"

#include "PageReferences.h"
#include "PageReplacementExecutor.h"
#include "PageReplacementExecutorListener.h"
#include "FrameProvider.h"

#include "TableView.h"
#include "TableElement.h"
#include "AboutDialog.h"


static const QColor COLOR_HEADER(219, 229, 241);
static const QColor COLOR_FRAMES(194, 214, 155);
static const QColor COLOR_PFAULT(251, 212, 180);
#include "Page.h"
#include <QColor>

class ImplPageReplacementExecutorListener : public PageReplacementExecutorListener
{
	int cols;
	int rows;
	QVector<TableElement*> elems;
	int currentColumn;
	int		pageFaults;
public:
	ImplPageReplacementExecutorListener() : currentColumn(1)
	{
	}
	~ImplPageReplacementExecutorListener()
	{
	}
	void init(FrameProvider& frames, PageReferences& references)
	{
		cols = 1+references.count();
		rows = 1+frames.count()+1;
		elems.resize(cols*rows);
		pageFaults = 0;

		//
		// Celda nula
		//
		elems[0] = new TableElement(Qt::black);
		//
		// Celda de faltas
		//
		elems[(frames.count()+2-1)*cols] = new TableElement("FP", COLOR_FRAMES);
		//
		// Celdas de marcos
		//
		for(int f = 0; f < frames.count(); f++)
		{
			elems[(f+1)*cols] = new TableElement(QString().sprintf("m%d", frames[f]->num), COLOR_FRAMES);
		}
	}
	void step(bool fp, Page* page, FrameProvider& frames, void* pointer)
	{
		int idx;

		// Poner referencia
		idx = currentColumn;
		elems[idx] = new TableElement(page->num, COLOR_HEADER);

		//
		// Rellenar filas
		//
		for(int f = 0; f < frames.count(); f++)
		{
			idx = ((f+1)*cols)+currentColumn;

			/*
			QString txt;
			if(frames[f]->page != NULL)
			{
				txt = QString().sprintf("%d", frames[f]->page->num);
				if(false)
				{
					if(frames[f]->page->refbit.isValid())
					{
						txt += QString().sprintf("(%d)", (int)frames[f]->page->refbit);
					}
				}
				else
				{
					if(frames[f]->page->refbit.isValid() && frames[f]->page->refbit == 1)
						txt += "*";
					else
						txt += " ";
				}
			}
			elems[idx] = new TableElement(txt, Qt::white);
			*/
			QString txtNum;
			QString txtBit;

			if(frames[f]->page != NULL)
			{
				txtNum = QString().sprintf("%d", frames[f]->page->num);

				if(frames[f]->page->refbit.isValid())
				{
					txtBit += QString().sprintf("%d", (int)frames[f]->page->refbit);
				}
			}
			elems[idx] = new TableElement(txtNum, Qt::white, Qt::black, txtBit);
		}
		//
		// Rellenar fallos de paginas
		//
		idx = ((1+frames.count())*cols)+currentColumn;
		elems[idx] = new TableElement(fp ? "X" : NULL, Qt::white, Qt::red);
		// ...
		currentColumn++;
		// Incrementar el contador de fallos de páginas
		if(fp)
		{
			pageFaults++;
		}
	}
	int getCols() const
	{
		return this->cols;
	}
	int getRows() const
	{
		return this->rows;
	}
	const QVector<TableElement*>& getElems() const
	{
		return this->elems;
	}
	int getPageFaults() const
	{
		return pageFaults;
	}
};


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// ...
	needReset = false;

	//
	// Cargar algoritmos
	//
	algorithms.append(new PageReplacementAlgolFIFO());
	algorithms.append(new PageReplacementAlgolLRU());
	algorithms.append(new PageReplacementAlgolOPT());
	algorithms.append(new PageReplacementAlgolCLOCK());
	for(int i = 0; i < algorithms.size(); i++)
	{
		ui->cboAlgorithms->addItem(algorithms.at(i)->name());
	}
	//
	// Crear visor de tabla
	//
	tableView = new TableView(this);
	tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->verticalLayout->addWidget(tableView);
}

MainWindow::~MainWindow()
{
	delete ui;
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
void MainWindow::slotExecute()
{
	if(needReset)
	{
		needReset = false;
		ui->txtFrames->setEnabled(true);
		ui->txtSequence->setEnabled(true);
		ui->btnRandom->setEnabled(true);
		ui->cboAlgorithms->setEnabled(true);
		ui->btnExecute->setText("Ejecutar");

		ui->statusBar->clearMessage();
	}
	else
	{
		int idx = ui->cboAlgorithms->currentIndex();

		if(idx >= 0 && idx < algorithms.size())
		{
			int frms = ui->txtFrames->text().toInt();
			PageReferences refs(ui->txtSequence->text());
			PageReplacementExecutor exec;
			ImplPageReplacementExecutorListener listener;

			exec.exec(algorithms.at(idx), &listener, frms, refs);

			tableView->setElements(listener.getCols(), listener.getRows(), listener.getElems());

			ui->statusBar->showMessage(tr("%1 fallos de páginas").arg(listener.getPageFaults()));

			needReset = true;
			ui->txtFrames->setEnabled(false);
			ui->txtSequence->setEnabled(false);
			ui->btnRandom->setEnabled(false);
			ui->cboAlgorithms->setEnabled(false);
			ui->btnExecute->setText("Editar");
		}
	}
}
#include <QInputDialog>
void MainWindow::slotRandomSequence()
{
	QString txt = QInputDialog::getText(this, "Sequencia aleatoria", "Cantidad de referencias:");

	if(!txt.isEmpty())
	{
		int num = txt.toInt();
		QString refs;
		int minValue = 1;
		int maxValue = 9;

		qsrand(qrand());
		for(int i = 0; i < num; i++)
		{
			if(!refs.isEmpty())
				refs += ",";
			refs += QString::number(qrand() % ((maxValue + 1) - minValue) + minValue);
		}
		ui->txtSequence->setText(refs);
	}
}



