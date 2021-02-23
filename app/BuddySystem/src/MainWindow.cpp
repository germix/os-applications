#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QListView>
#include <QListWidget>
#include <QMessageBox>

#include "BuddyNode.h"
#include "HelpDialog.h"
#include "AboutDialog.h"

#include "Lexer.h"
#include "BuddySystemCanvas.h"
#include "BuddySystemCanvas2.h"

QString formatBytesToSize(float bytes);

#define MEM_1MB 0x100000
#define MEM_4MB 0x400000

#include <QMouseEvent>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow), system(MEM_1MB)
{
	ui->setupUi(this);

	treeCanvas = new BuddySystemCanvas(&system);
	layoutCanvas = new BuddySystemCanvas2(&system);

	ui->verticalLayout1->addWidget(new QLabel(tr("Representación en árbol:")));
	ui->verticalLayout1->addWidget(treeCanvas);
	ui->verticalLayout1->addWidget(new QLabel(tr("Representación en fragmentos:")));
	ui->verticalLayout1->addWidget(layoutCanvas);

	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slotLineReturnPressed()));

	reset(MEM_1MB);
	ui->lineEdit->setFocus();
}

MainWindow::~MainWindow()
{
	disconnect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slotLineReturnPressed()));
	delete ui;
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
void MainWindow::slotLineReturnPressed()
{
	Token tok;
	Lexer lex(ui->lineEdit->text());

	tok = lex.nextToken();
	//while(tok != TOK_EOF)
	{
		if(tok == TOK_MEMORY)
		{
			alloc(tok.size);
		}
		else if(tok != TOK_IDENT)
		{
			// ERROR
		}
		else
		{
			QString s = tok.lexeme;

			if(s == "free")
			{
				tok = lex.nextToken();
				if(tok == TOK_IDENT)
				{
					free(tok.lexeme);
				}
			}
			else if(s == "reset")
			{
				tok = lex.nextToken();
				reset(tok.size);
			}
			else if(s == "alloc")
			{
				tok = lex.nextToken();
				if(tok == TOK_MEMORY)
				{
					alloc(tok.size);
				}
			}
			else
			{
				tok = lex.nextToken();
				if(tok == '=')
				{
					tok = lex.nextToken();
					if(tok.lexeme == "alloc")
					{
						tok = lex.nextToken();
						if(tok == TOK_MEMORY)
						{
							alloc(tok.size, s);
						}
					}
				}
				else
				{
					// ERROR
				}
			}
		}
	}
}
void MainWindow::showMessage()
{
	int vFree = system.memTotal-system.memUsed;
	int vUsed = system.memUsed;
	int vAllocated = system.memAllocated;
	int vWasted = vUsed-vAllocated;
	QString status = QString("Espacio: libre=%1, ocupado=%2, reservado=%3, desperdiciado=%4");

	status = status.arg(
			formatBytesToSize(vFree),
			formatBytesToSize(vUsed),
			formatBytesToSize(vAllocated),
			formatBytesToSize(vWasted));

	ui->statusBar->showMessage(status);
}
void MainWindow::reset(int size)
{
	system.init(size);
	treeCanvas->rebuild();
	layoutCanvas->rebuild();
	ui->lineEdit->clear();;
	ui->treeWidget->clear();
	ui->listWidget->clear();

	ui->listWidget->addItem("Memoria inicial: " + formatBytesToSize(size));

	showMessage();
}

void MainWindow::free(const QString& name)
{
	if(system.free(name))
	{
		treeCanvas->rebuild();
		layoutCanvas->rebuild();
		const QList<QTreeWidgetItem*>& f = ui->treeWidget->findItems(name, Qt::MatchExactly);
		for(int i = 0; i < f.size(); i++)
		{
			delete f.at(i);
		}
		ui->lineEdit->clear();
		ui->listWidget->addItem("Se eliminó \"" + name + "\"");

		showMessage();
	}
	else
	{
		QMessageBox::information(NULL, NULL, "Variable no encontrada");
		ui->lineEdit->setFocus();
	}
}
void MainWindow::alloc(int size, const QString& name)
{
	BuddyNode* node;

	if(NULL != (node = system.alloc(size, name)))
	{
		treeCanvas->rebuild();
		layoutCanvas->rebuild();
		// ...
		new QTreeWidgetItem
		(
			ui->treeWidget,
			QStringList()
			<< node->getName()
			<< formatBytesToSize(size)
			<< formatBytesToSize(node->getSize())
		);
		ui->lineEdit->clear();
		ui->listWidget->addItem("Se reservó \"" + node->getName() + "\" con " + formatBytesToSize(size));

		showMessage();
	}
	else
	{
		QMessageBox::information(NULL, NULL, "No hay mas espacio de memoria");
		ui->lineEdit->setFocus();
	}
}
/*
bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
	static BuddyNode* node = NULL;

	if(watched == ui->treeWidget)
	{
		if(event->type() == QEvent::MouseMove)
		{
			QTreeWidgetItem* item = ui->treeWidget->itemAt(((QMouseEvent*)event)->pos());
			if(item != NULL)
			{
				BuddyNode* found = system.find(item->text(0));
				if(found == NULL)
				{
					if(node != NULL)
					{
						node->highlight = false;
						treeCanvas->update();
						layoutCanvas->update();
					}
				}
				else
				{
					if(node != found)
					{
						node->highlight = true;
						treeCanvas->update();
						layoutCanvas->update();
					}
				}
				node = found;
			}
		}
	}
	return QMainWindow::eventFilter(watched, event);
}
*/



