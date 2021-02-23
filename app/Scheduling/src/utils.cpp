#include <QColor>
#include <QString>
#include <QVector>

void fillColorsVector(const QString& text, QVector<QColor>& colors)
{
	int mode = 0;

	colors.clear();
	for(int i = 0; i < text.size(); i++)
	{
		QChar ch = text.at(i);

		if(ch == ',')
		{
			mode = !mode;
			colors.append(Qt::black);
		}
		else
		{
			if(mode == 0)
				colors.append(Qt::red);
			else
				colors.append(Qt::blue);
		}
	}
}

