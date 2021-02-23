#ifndef PAGELISTLRU_H
#define PAGELISTLRU_H

class Page;

class PageNodeLRU
{
public:
	Page* page;
	PageNodeLRU* prev;
	PageNodeLRU* next;
public:
	PageNodeLRU(Page* p) : page(p), prev(0), next(0)
	{
	}
};

class PageListLRU
{
	PageNodeLRU* head;
	PageNodeLRU* tail;
public:
	PageListLRU();
	~PageListLRU();
public:
	void clear();
	void update(Page* p);
	Page* getLeastRecentlyUsed();
};

#endif // PAGELISTLRU_H
