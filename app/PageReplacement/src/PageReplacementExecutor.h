#ifndef PAGEREPLACEMENTEXECUTOR_H
#define PAGEREPLACEMENTEXECUTOR_H

class PageReferences;
class PageReplacement;
class PageReplacementExecutorListener;

class PageReplacementExecutor
{
public:
	PageReplacementExecutor();
public:
	void exec(PageReplacement* algo, PageReplacementExecutorListener* listener, int maxFrames, PageReferences& references);
};

#endif // PAGEREPLACEMENTEXECUTOR_H
