#include "PageReferences.h"
#include <QString>
#include <QStringList>

PageReferences::PageReferences() : references(0), referencesCount(0)
{
}
PageReferences::PageReferences(const QString& txt) : references(0), referencesCount(0)
{
	QStringList l = txt.split(",");
	if(!l.isEmpty())
	{
		referencesCount = l.size();
		references = new int[referencesCount];
		for(int i = 0; i < referencesCount; i++)
		{
			references[i] = l.at(i).toInt();
		}
	}
}
PageReferences::~PageReferences()
{
	if(references)
		delete [] references;
}
