#ifndef PAGEREPLACEMENTALGOLCLOCK_H
#define PAGEREPLACEMENTALGOLCLOCK_H
#include "PageReplacement.h"

class PageReplacementAlgolCLOCK : public PageReplacement
{
public:
	PageReplacementAlgolCLOCK() {}
	~PageReplacementAlgolCLOCK() {}
public:
	virtual QString name() const;
	virtual void    exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames);
};

#endif // PAGEREPLACEMENTALGOLCLOCK_H
