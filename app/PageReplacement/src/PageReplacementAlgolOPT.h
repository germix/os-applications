#ifndef PAGEREPLACEMENTALGOLOPT_H
#define PAGEREPLACEMENTALGOLOPT_H
#include "PageReplacement.h"

class PageReplacementAlgolOPT : public PageReplacement
{
public:
	PageReplacementAlgolOPT() {}
	~PageReplacementAlgolOPT() {}
public:
	virtual QString name() const;
	virtual void    exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames);
};

#endif // PAGEREPLACEMENTALGOLOPT_H
