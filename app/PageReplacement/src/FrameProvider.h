#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H
#include "Frame.h"

class FrameProvider
{
	Frame* frames;
	int    framesCount;
public:
	FrameProvider(int maxFrames);
	~FrameProvider();
public:
	int count() const
	{
		return framesCount;
	}
	Frame* operator [] (int i) const
	{
		return &frames[i];
	}
};

#endif // FRAMEPROVIDER_H
