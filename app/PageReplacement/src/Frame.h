#ifndef FRAME_H
#define FRAME_H

class Page;

class Frame
{
public:
	int num;
	Page* page;
public:
	Frame() : num(-1), page(0)
	{
	}
	Frame(int n) : num(n), page(0)
	{
	}
};

#endif // FRAME_H
