#ifndef WORKINGSET_H
#define WORKINGSET_H
#include <QVector>

class PageWS;
class ModelWS;
class WorkingSetListener;

class PageWS
{
public:
	int ref;			// Cantidad de referencias
	int numPage;		// Número de página
public:
	PageWS(int num) : ref(0), numPage(num)
	{
	}
};



class WorkingSetListener
{
public:
	WorkingSetListener() {}
	virtual ~WorkingSetListener() {}
public:
	virtual void init() = 0;
	virtual void step(const QVector<PageWS*>& wset, int tail, int head) = 0;
};

class WorkingSet
{
	int					tail;
	int					index;

	int					wsize;
	QVector<int>		sequence;

	QList<PageWS*>		wset;
	int					pageFaults;

	QVector<PageWS*>	allPages;

	WorkingSetListener*	listener;

public:
	WorkingSet();
	~WorkingSet();
public:
	void init(WorkingSetListener* l, const QVector<int>& seq, int wsize);
	void step();
private:
	void clear();

	void addPage(int numPage)
	{
		pageFaults++;

		PageWS* p = allPages.at(numPage);
		p->ref++;
		wset.append(p);
	}
	PageWS* findPage(int numPage)
	{
		PageWS* p;
		QList<PageWS*>::const_iterator iter = wset.constBegin();

		for(iter = wset.constBegin(); iter != wset.constEnd(); iter++)
		{
			p = *iter;
			if(p->numPage == numPage)
				return p;
		}
		return NULL;
	}
	void unrefPage(int numPage)
	{
		PageWS* pws = findPage(numPage);
		if(NULL != pws)
		{
			if(--pws->ref == 0)
			{
				wset.removeOne(pws);
			}
		}
	}
public:
	bool atEnd() const
	{
		return !(index < sequence.size());
	}
	const QVector<PageWS*> getPages() const
	{
		return allPages;
	}
	const QVector<PageWS*> getWorkingSet() const;
	int getPageFaults() const
	{
		return pageFaults;
	}
	int getWindowStart() const { return tail; }
	int getWindowEnd() const { return index; }
};

#endif // WORKINGSET_H
