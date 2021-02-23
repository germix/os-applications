#ifndef PAGE_H
#define PAGE_H
#include "ReferenceBit.h"

class Frame;

class Page
{
public:
	int num;
	Frame* frame;
	ReferenceBit refbit;
public:
	Page() : num(-1), frame(0)
	{
	}
	Page(int n) : num(n), frame(0)
	{
	}
};

#endif // PAGE_H
