#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H
#include "Page.h"

class Page;
class PageReferences;

class PageProvider
{
	Page* pages;
	int   count;
public:
	PageProvider(PageReferences& references);
	~PageProvider();
public:
	Page* findPage(int num);
	Page* findPage(int num, bool* pageFault);
};

#endif // PAGEPROVIDER_H
