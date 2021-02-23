#ifndef PAGEREFERENCES_H
#define PAGEREFERENCES_H

class QString;

class PageReferences
{
	int* references;
	int  referencesCount;
public:
	PageReferences();
	PageReferences(const QString& txt);
	~PageReferences();
public:
	int at(int i) const
	{
		return references[i];
	}
	int count() const
	{
		return referencesCount;
	}
	int operator [] (int i) const
	{
		return references[i];
	}
};

#endif // PAGEREFERENCES_H
