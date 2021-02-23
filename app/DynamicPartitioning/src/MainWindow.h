#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>

namespace Ui {
    class MainWindow;
}

class Canvas;
class QLineEdit;
class QTreeWidget;
class QComboBox;
class QTreeView;

#include "MemoryManager.h"
#include "ChunkModel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

	Canvas*			canvas;
	QLineEdit*		lineEdit;
	QComboBox*		comboBox;

	MemoryManager			mmgr;
	QHash<QString, Chunk*>	pointers;

	ChunkModel*				model;
	QTreeView*				treeView;

	int						lastSize;
	QString					lastFileName;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
	void slotFileNew();
	void slotFileOpen();
	void slotFileSave();
	void slotFileSaveAs();
	void slotFileExit();
	void slotHelpHelp();
	void slotHelpAbout();
	void slotReset();
	void slotReturnPressed();
private:
	void free(const QString& name);
	Chunk* alloc(int size, const QString& name, const QColor& color);
};

#endif // MAINWINDOW_H
