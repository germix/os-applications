#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "BuddySystem.h"

namespace Ui {
class MainWindow;
}

class QLineEdit;

class BuddySystemCanvas;
class BuddySystemCanvas2;

class QListWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Ui::MainWindow*			ui;

	BuddySystem				system;
	BuddySystemCanvas*		canvas;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void slotFileExit();
	void slotHelpHelp();
	void slotHelpAbout();
	void slotLineReturnPressed();
private:
	void showMessage();
	void reset(int size);
	void free(const QString& name);
	void alloc(int size, const QString& name = "");
};

#endif // MAINWINDOW_H
