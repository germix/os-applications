#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Canvas.h"
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

#include <time.h>
#include "Lexer.h"
#include "HelpDialog.h"
#include "AboutDialog.h"

QString formatBytesToSize(float bytes);

static bool parseColor(Lexer& lex, Token& tok, QColor& color)
{
	if(tok == TOK_IDENT)
	{
		if(tok.lexeme == "red")				color = QColor(Qt::red);
		else if(tok.lexeme == "blue")		color = QColor(Qt::blue);
		else if(tok.lexeme == "green")		color = QColor(Qt::green);
		else if(tok.lexeme == "cyan")		color = QColor(Qt::cyan);
		else if(tok.lexeme == "magenta")	color = QColor(Qt::magenta);
		else if(tok.lexeme == "yellow")		color = QColor(Qt::yellow);
		else
		{
			return false;
		}
	}
	else if(tok == '(')
	{
		tok = lex.nextToken();	// Saltar '('

		int i = 0;
		int values[3];
		while(tok != ')' && tok != TOK_EOF)
		{
			if(tok == ',')
			{
				i++;
				if(i == 3) // Demasiados parámetros
				{
					return false;
				}
			}
			else if(tok == TOK_NUMERIC)
			{
				values[i] = tok.size;
			}
			tok = lex.nextToken();
		}
		if(tok != ')')
		{
			return false;
		}
		color = QColor(values[0], values[1], values[2]);
		tok = lex.nextToken();
	}
	return true;
}
static bool parseAlloc(Lexer& lex, Token& tok, int& size, QString& name, QColor& color)
{
	if(tok.lexeme == "alloc")
	{
		tok = lex.nextToken();
		if(tok == TOK_MEMORY)
		{
			size = tok.size;
			// ...
			tok = lex.nextToken();
			while(tok == ',')
			{
				tok = lex.nextToken();
				if(tok.lexeme == "name")
				{
					tok = lex.nextToken();
					if(tok == '=')
					{
						tok = lex.nextToken();
						if(tok == TOK_IDENT)
							name = tok.lexeme;
					}
				}
				else if(tok.lexeme == "color")
				{
					tok = lex.nextToken();
					if(tok == '=')
					{
						tok = lex.nextToken();
						parseColor(lex, tok, color);
					}
				}
				else
				{
					break;
				}
				tok = lex.nextToken();
			}
			return true;
		}
		else
		{
			// TODO: error
		}
	}
	return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	// ...
	lastSize = 0x800000;
	mmgr.init(lastSize);
	// ...
	canvas = new Canvas(&mmgr);
	lineEdit = new QLineEdit();
	comboBox = new QComboBox();
	treeView = new QTreeView();

	QWidget* central = new QWidget();
	QVBoxLayout* vl = new QVBoxLayout();
	QHBoxLayout* hl1 = new QHBoxLayout();
	QHBoxLayout* hl2 = new QHBoxLayout();

	model = new ChunkModel(&mmgr);
	treeView->setModel(model);
	treeView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	treeView->setColumnWidth(0, 80);
	treeView->setColumnWidth(1, 80);
	treeView->setColumnWidth(2, 10);
	treeView->setRootIsDecorated(false);
	treeView->setMinimumWidth(220);
	treeView->setMaximumWidth(220);
	comboBox->addItems(QStringList() << "Primer ajuste" << "Mejor ajuste" << "Peor ajuste");

	hl1->addWidget(canvas);
	hl1->addWidget(treeView);
	hl2->addWidget(lineEdit);
	hl2->addWidget(new QLabel("Estrategia:"));
	hl2->addWidget(comboBox);
	vl->addLayout(hl1);
	vl->addLayout(hl2);
	central->setLayout(vl);
	setCentralWidget(central);

	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
	lineEdit->setFocus();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::slotFileNew()
{
	QString value = QInputDialog::getText(this, tr("Tamaño"), tr("Tamaño:"));
	if(!value.isEmpty())
	{
		Lexer lex(value);
		Token tok = lex.nextToken();

		if(tok == TOK_MEMORY)
		{
			model->init(tok.size);
			canvas->viewport()->update();
			lastSize = tok.size;
			lastFileName = QString();
		}
	}
}
void MainWindow::slotFileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), QString(), tr("Archivos de particiones (*.xml)"));
	if(!fileName.isEmpty())
	{
		if(model->load(fileName))
		{
			canvas->reset();
			lineEdit->clear();
			lineEdit->setFocus();
			// ...
			lastFileName = fileName;
		}
	}
}
void MainWindow::slotFileSave()
{
}
void MainWindow::slotFileSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), QString(), tr("Archivos de particiones (*.xml)"));
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
void MainWindow::slotReset()
{
	if(lastFileName.isEmpty())
		model->init(lastSize);
	else
		model->load(lastFileName);
	// ...
	canvas->viewport()->update();
}
void MainWindow::slotReturnPressed()
{
	Token tok;
	Lexer lex(lineEdit->text());

	tok = lex.nextToken();

	if(tok == TOK_IDENT)
	{
		if(tok.lexeme == "free")
		{
			tok = lex.nextToken();
			if(tok == TOK_IDENT)
			{
				free(tok.lexeme);
			}
			else
			{
				// TODO: error
			}
		}
		else
		{
			int size;
			QString name;
			QColor  color;

			if(parseAlloc(lex, tok, size, name, color))
			{
				//
				// Comprobar nombre
				//
				if(name.isEmpty())
				{
					static int s = 123456789;
					do
					{
						s += time(0);
						qsrand(s);

						name = "ptr" + QString::number(qrand());
					}
					while(NULL != mmgr.find(name));
				}
				if(NULL != mmgr.find(name))
				{
					int idx = 1;
					while(NULL != mmgr.find(name+QString::number(idx)))
					{
						idx++;
					}
					name += QString::number(idx);
				}
				//
				// ...
				//
				alloc(size, name, color);
			}
		}
	}
}
void MainWindow::free(const QString& name)
{
	if(model->free(name))
	{
		canvas->viewport()->update();
	}
}
Chunk* MainWindow::alloc(int size, const QString& name, const QColor& color)
{
	Chunk* c = model->alloc(comboBox->currentIndex(), size, name, color);
	if(c != NULL)
	{
		canvas->viewport()->update();
		return c;
	}
	else
	{
		QMessageBox::information(NULL, NULL, "No hay mas espacio de memoria");
		lineEdit->setFocus();
	}
	return NULL;
}

