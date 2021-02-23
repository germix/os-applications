#include "MyItemModel.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

MyItemModel::MyItemModel() : QStandardItemModel(0, 2), enumerateFromZero(false)
{
	rulerSteps = 1;
	setHeaderData(0, Qt::Horizontal, QString("Tiempo de llegada"), Qt::DisplayRole);
	setHeaderData(1, Qt::Horizontal, QString("Tiempos de procesamiento (CPU, E/S)"), Qt::DisplayRole);
}

MyItemModel::MyItemModel(bool enumFromZero, int process) : QStandardItemModel(process, 2), enumerateFromZero(enumFromZero)
{
	rulerSteps = 1;
	for(int i = 0; i < process; i++)
	{
		setItem(i, 0, new QStandardItem("0"));
		if(enumerateFromZero)
			setHeaderData(i, Qt::Vertical, QString().sprintf("P%d", i), Qt::DisplayRole);
		else
			setHeaderData(i, Qt::Vertical, QString().sprintf("P%d", i+1), Qt::DisplayRole);
	}
	setHeaderData(0, Qt::Horizontal, QString("Tiempo de llegada"), Qt::DisplayRole);
	setHeaderData(1, Qt::Horizontal, QString("Tiempos de procesamiento (CPU, E/S)"), Qt::DisplayRole);
}
MyItemModel::~MyItemModel()
{
}

void MyItemModel::addProcess(int at, const QString& bursts)
{
	int r = rowCount();
	insertRow(r);
	setItem(r, 0, new QStandardItem(QString::number(at)));
	setItem(r, 1, new QStandardItem(bursts));
}
void MyItemModel::addProcess(const QString& at, const QString& bursts)
{
	int r = rowCount();
	insertRow(r);
	setItem(r, 0, new QStandardItem(at));
	setItem(r, 1, new QStandardItem(bursts));
}
void MyItemModel::setProcess(int processIndex, int at, const QString& bursts)
{
	setItem(processIndex, 0, new QStandardItem(QString::number(at)));
	setItem(processIndex, 1, new QStandardItem(bursts));
}
void MyItemModel::removeProcess(int processIndex)
{
	removeRow(processIndex);
}
int MyItemModel::getInitialIndex() const
{
	return (enumerateFromZero ? 0 : 1);
}
void MyItemModel::setInitialIndex(int index)
{
	if(index == 0)
		enumerateFromZero = true;
	else
		enumerateFromZero = false;

	headerDataChanged(Qt::Vertical, 0, rowCount()-1);
}

QVariant MyItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(orientation == Qt::Vertical && role == Qt::DisplayRole)
	{
		if(enumerateFromZero)
			return "P"+QString::number(section);
		else
			return "P"+QString::number(section+1);
	}
	return QStandardItemModel::headerData(section, orientation, role);
}
bool MyItemModel::open(const QString& fname)
{
	bool ok = false;
	QFile file(fname);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QXmlStreamReader reader(&file);

		//
		// Parsear documento
		//
		while(!reader.atEnd() && !reader.hasError())
		{
			QXmlStreamReader::TokenType token = reader.readNext();

			if(token == QXmlStreamReader::StartDocument)
			{
			}
			else if(token == QXmlStreamReader::StartElement)
			{
				if(reader.name() == "Scheduling")
				{
					QXmlStreamAttributes attrs = reader.attributes();
					QStringRef initialIndex = attrs.value("initialIndex");

					description = attrs.value("description").toString();
					if(initialIndex.isEmpty())
					{
#if (QT_VERSION >= 0x050000)
						enumerateFromZero = initialIndex.toInt() == 0 ? true : false;
#else
						enumerateFromZero = initialIndex.toString().toInt() == 0 ? true : false;
#endif
					}
					QStringRef rs = attrs.value("rulerSteps");
					if(rs.isEmpty())
						rulerSteps = 1;
					else
						rulerSteps = rs.toString().toInt();

				}
				else if(reader.name() == "Process")
				{
					QXmlStreamAttributes attrs = reader.attributes();
					QStringRef at = attrs.value("at");
					QStringRef bursts = attrs.value("bursts");
					addProcess(at.toString(), bursts.toString());
				}
			}
		}
		//
		// Comprobar que no haya errores
		//
		if(!reader.hasError())
		{
			ok = true;
			filename = fname;
		}
		//
		// Cerrar archivo
		//
		file.close();
	}
	return ok;
}
bool MyItemModel::save(const QString& fname)
{
	bool ok = false;
	QFile file(fname);
	if(file.open(QIODevice::WriteOnly))
	{
		QXmlStreamWriter writer(&file);

		writer.setAutoFormatting(true);
		writer.writeStartDocument();

		//
		// Escribir header
		//
		writer.writeStartElement("Scheduling");
		writer.writeAttribute("description", description);
		writer.writeAttribute("rulerSteps", QString::number(rulerSteps));
		writer.writeAttribute("initialIndex", QString::number(getInitialIndex()));

		//
		// Escribir procesos
		//
		{
			for(int i = 0; i < rowCount(); i++)
			{
				writer.writeStartElement("Process");
				writer.writeAttribute("at", item(i, 0)->text());
				writer.writeAttribute("bursts", item(i, 1)->text());
				writer.writeEndElement();
			}
		}
		// Fin del header
		writer.writeEndElement();

#if (QT_VERSION >= 0x050000)
		if(!writer.hasError())
#else
#endif
		{
			ok = true;
			filename = fname;
		}

		//
		// Cerrar archivo
		//
		file.close();
	}
	return ok;
}


