#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class TableView;
class PageReplacement;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	QVector<PageReplacement*>		algorithms;

	TableView*						tableView;
	bool							needReset;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
public slots:
	void slotFileExit();
	void slotHelpAbout();
	void slotExecute();
	void slotRandomSequence();
};

#endif // MAINWINDOW_H
