#include "FrameProvider.h"

#include "Frame.h"

FrameProvider::FrameProvider(int maxFrames) : frames(0), framesCount(0)
{
	//
	// Generar marcos
	//
	if(maxFrames > 0)
	{
		frames = new Frame[maxFrames];
		for(int i = 0; i < maxFrames; i++)
			frames[i] = Frame(i+1);
		framesCount = maxFrames;
	}
}
FrameProvider::~FrameProvider()
{
	//
	// Eliminar marcos
	//
	if(frames)
		delete [] frames;
}
