#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
#include "WorkingSet.h"


class QTreeWidgetItem;
class WorkingSetCanvas;

class MainWindow : public QMainWindow
{
    Q_OBJECT

	bool						invalid;
	QVector<QTreeWidgetItem*>	treeItems;

	WorkingSet					wsetModel;
	WorkingSetListener*			wsetListener;

	WorkingSetCanvas*			canvas;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
	void slotRun();
	void slotFileExit();
	void slotHelpAbout();
	void slotTextChanged(const QString& text);

	void slotCancel();
private:
	QVector<int> getSequence();
	int getWindowSize();
public:
	bool eventFilter(QObject* watched, QEvent* event);
};

#endif // MAINWINDOW_H
