#ifndef PAGEREPLACEMENTALGOLFIFO_H
#define PAGEREPLACEMENTALGOLFIFO_H
#include "PageReplacement.h"

class PageReplacementAlgolFIFO : public PageReplacement
{
public:
	PageReplacementAlgolFIFO() {}
	~PageReplacementAlgolFIFO() {}
public:
	virtual QString name() const;
	virtual void    exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames);
};

#endif // PAGEREPLACEMENTALGOLFIFO_H
