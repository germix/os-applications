#ifndef PAGEREPLACEMENTEXECUTORLISTENER_H
#define PAGEREPLACEMENTEXECUTORLISTENER_H

class Page;
class FrameProvider;

class PageReplacementExecutorListener
{
public:
	PageReplacementExecutorListener() {}
	virtual ~PageReplacementExecutorListener() {}
public:
	virtual void init(FrameProvider& frames, PageReferences& references) = 0;
	virtual void step(bool fp, Page* page, FrameProvider& frames, void* pointer) = 0;
};

#endif // PAGEREPLACEMENTEXECUTORLISTENER_H
