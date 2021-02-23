#ifndef PAGEREPLACEMENT_H
#define PAGEREPLACEMENT_H
#include <QString>

class PageProvider;
class PageReferences;
class FrameProvider;
class PageReplacementExecutorListener;

class PageReplacement
{
public:
	PageReplacement() {}
	virtual ~PageReplacement() {}
public:
	virtual QString name() const = 0;
	virtual void    exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames) = 0;
};

#endif // PAGEREPLACEMENT_H
