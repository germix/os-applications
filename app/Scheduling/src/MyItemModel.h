#ifndef MYITEMMODEL_H
#define MYITEMMODEL_H
#include <QStandardItemModel>

class MyItemModel : public QStandardItemModel
{
	bool enumerateFromZero;

	QString			filename;
	QString			description;
	int				rulerSteps;
public:
	MyItemModel();
	MyItemModel(bool enumFromZero, int process = 0);
	~MyItemModel();
public:
	void addProcess(int at = 0, const QString& bursts = "");
	void addProcess(const QString& at, const QString& bursts = "");
	void setProcess(int processIndex, int at, const QString& bursts);

	void removeProcess(int processIndex);

	int getBurstsColumn() const { return 1; }
	int getArrivalColumn() const { return 0; }
	int getInitialIndex() const;
	void setInitialIndex(int index);
	bool isEnumFromZero() const { return enumerateFromZero; }

	void setDescripcion(const QString& descr) { description = descr; }
	QString getDescription() const { return description; }

	bool hasFilename() const { return !filename.isEmpty(); }
	QString getFilename() const { return filename; }

	int getRulerSteps() const { return rulerSteps; }
	void setRulerSteps(int steps)
	{
		rulerSteps = steps;
	}
public:
	bool open(const QString& fname);
	bool save(const QString& fname);
protected:
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // MYITEMMODEL_H
