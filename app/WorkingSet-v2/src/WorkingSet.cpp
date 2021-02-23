#include "WorkingSet.h"
#include <QSet>

WorkingSet::WorkingSet()
{
}
WorkingSet::~WorkingSet()
{
	clear();
}
void WorkingSet::clear()
{
	PageWS* p;
	QVector<PageWS*>::const_iterator iter;

	for(iter = allPages.constBegin(); iter != allPages.constEnd(); iter++)
	{
		p = *iter;
		delete p;
	}
	wset.clear();
	allPages.clear();
	sequence.clear();
}
void WorkingSet::init(WorkingSetListener* l, const QVector<int>& seq, int wsize)
{
	this->clear();
	this->wsize = wsize;
	this->sequence = seq;
	this->tail = 0;
	this->index = 0;
	this->pageFaults = 0;
	this->listener = l;

	//
	// Generar páginas
	//
	int max = -1;
	for(int i = 0; i < sequence.size(); i++)
	{
		max = qMax(max, sequence.at(i));
	}
	for(int i = 0; i < max+1; i++)
	{
		allPages.append(new PageWS(i));
	}
}

void WorkingSet::step()
{
	if(index < sequence.size())
	{
		int p = sequence.at(index);
		PageWS* pws = findPage(p);

		if(pws == NULL)
			addPage(p);
		else
			pws->ref++;

		if((index-tail) >= wsize)
		{
			unrefPage(sequence[tail]);
			tail++;
		}
		//
		// Notificar
		//
		{
			QVector<PageWS*> pages;
			QList<PageWS*>::const_iterator iter;

			for(iter = wset.constBegin(); iter != wset.constEnd(); iter++)
			{
				pages.append(*iter);
			}
			listener->step(pages, tail, index);
		}
		// ...
		index++;
	}
}


const QVector<PageWS*> WorkingSet::getWorkingSet() const
{
}


