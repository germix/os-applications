#include "WorkingSet.h"
#include <QSet>

class PageWS
{
public:
	int ref;			// Cantidad de referencias
	int numPage;		// Número de página
public:
	PageWS(int num) : ref(1), numPage(num)
	{
	}
};

class ModelWS
{
	QList<PageWS*>	wset;
	int				pageFaults;
public:
	ModelWS() : pageFaults(0)
	{
	}
	~ModelWS()
	{
		clear();
	}
public:
	void add(int numPage)
	{
		pageFaults++;
		wset.append(new PageWS(numPage));
	}
	PageWS* find(int numPage)
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
	void unref(int numPage)
	{
		PageWS* pws = find(numPage);
		if(NULL != pws)
		{
			if(--pws->ref == 0)
			{
				wset.removeOne(pws);
			}
		}
	}
	void show(int nro, WorkingSetListener* listener)
	{
		QVector<int> pages;
		PageWS* p;
		QList<PageWS*>::const_iterator iter;

		for(iter = wset.constBegin(); iter != wset.constEnd(); iter++)
		{
			p = *iter;
			pages.append(p->numPage);
		}
		listener->step(nro, pages);
	}
	void clear()
	{
		PageWS* p;
		QList<PageWS*>::const_iterator iter = wset.constBegin();

		for(iter = wset.constBegin(); iter != wset.constEnd(); iter++)
		{
			p = *iter;
			delete p;
		}
		wset.clear();
	}
	int getPageFaults() const
	{
		return pageFaults;
	}
};


WorkingSet::WorkingSet()
{
}
WorkingSet::~WorkingSet()
{
}
int WorkingSet::run(WorkingSetListener* listener, int wsize, QVector<int> sequence)
{
	int tail = 0;
	ModelWS mws;

	listener->init();
	for(int s = 0; s < sequence.size(); s++)
	{
		int p = sequence.at(s);
		PageWS* pws = mws.find(p);

		if(pws == NULL)
			mws.add(p);
		else
			pws->ref++;

		if((s-tail) >= wsize)
		{
			mws.unref(sequence[tail]);
			tail++;
		}
		mws.show(s+1, listener);
	}
	return mws.getPageFaults();
}
