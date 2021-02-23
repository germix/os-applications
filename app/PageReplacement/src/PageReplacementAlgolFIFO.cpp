#include "PageReplacementAlgolFIFO.h"

#include "PageProvider.h"
#include "PageReferences.h"
#include "FrameProvider.h"
#include "PageReplacementExecutorListener.h"

QString PageReplacementAlgolFIFO::name() const
{
	return "FIFO";
}
void    PageReplacementAlgolFIFO::exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames)
{
	bool fp;
	int frmIndex = 0;

	listener->init(frames, references);

	for(int r = 0; r < references.count(); r++)
	{
		Page* p = pages.findPage(references[r], &fp);

		if(fp)
		{
			//
			// Borrar referencia anterior
			//
			if(frames[frmIndex]->page != NULL)
				frames[frmIndex]->page->frame = NULL;

			//
			// Establecer nueva referencia
			//
			frames[frmIndex]->page = p;
			p->frame = frames[frmIndex];
			//
			// Avanzar
			//
			frmIndex++;
			if(frmIndex >= frames.count())
				frmIndex = 0;
		}
		listener->step(fp, p, frames, NULL);
	}
}
