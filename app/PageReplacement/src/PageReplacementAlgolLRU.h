#ifndef PAGEREPLACEMENTALGOLLRU_H
#define PAGEREPLACEMENTALGOLLRU_H
#include "PageReplacement.h"

class PageReplacementAlgolLRU : public PageReplacement
{
public:
	PageReplacementAlgolLRU() {}
	~PageReplacementAlgolLRU() {}
public:
	virtual QString name() const;
	virtual void    exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames);
};

#endif // PAGEREPLACEMENTALGOLLRU_H
