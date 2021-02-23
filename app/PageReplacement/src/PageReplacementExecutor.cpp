#include "PageReplacementExecutor.h"

#include "PageProvider.h"
#include "FrameProvider.h"
#include "PageReferences.h"
#include "PageReplacement.h"

PageReplacementExecutor::PageReplacementExecutor()
{

}
void PageReplacementExecutor::exec(PageReplacement* algo, PageReplacementExecutorListener* listener, int maxFrames, PageReferences& references)
{
	PageProvider pages(references);
	FrameProvider frames(maxFrames);

	//
	// Ejecutar algoritmo
	//
	algo->exec(listener, references, pages, frames);

}



