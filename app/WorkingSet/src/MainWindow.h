#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
#include "WorkingSet.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

	WorkingSet					wset;
	WorkingSetListener*			wsetListener;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:
	bool eventFilter(QObject* watched, QEvent* event);
public slots:
	void slotRun();
	void slotFileExit();
	void slotHelpAbout();
	void slotTextChanged(const QString& text);
	void slotValueChanged(int value);
private:
	QVector<int> getSequence();
	int getWindowSize();
};

#endif // MAINWINDOW_H
