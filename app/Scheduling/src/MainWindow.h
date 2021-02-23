#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTableView;

class MyItemModel;
class MyItemDelegate;
class GantCanvas;

class Scheduling;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow*			ui;

	QTableView*				table;
	MyItemModel*			model;

	GantCanvas*				gantCanvas;

	QVector<Scheduling*>	algorithms;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void slotFileNew();
	void slotFileOpen();
	void slotFileSave();
	void slotFileSaveAs();
	void slotFileExit();
	void slotHelpHelp();
	void slotHelpAbout();
	void slotSimulatorExec();
	void slotSimulatorAddProcess();
	void slotSimulatorRemoveProcess();
	void slotSimulatorChangeInitialIndex();
	void slotSimulatorEditMarks();
	void slotSimulatorEditDescription();
private:
	MyItemModel* createModel(int process);
	void updateInitialIndexButton();
};

#endif // MAINWINDOW_H
