#include "PageReplacementAlgolLRU.h"

#include "PageListLRU.h"
#include "PageProvider.h"
#include "PageReferences.h"
#include "FrameProvider.h"
#include "PageReplacementExecutorListener.h"

QString PageReplacementAlgolLRU::name() const
{
	return "LRU";
}
void    PageReplacementAlgolLRU::exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames)
{
	bool fp;
	int frmIndex = 0;
	PageListLRU list;

	listener->init(frames, references);
	for(int s = 0; s < references.count(); s++)
	{
		Page* p = pages.findPage(references[s], &fp);

		if(fp)
		{
			Frame* f = frames[frmIndex];

			if(f->page == NULL)
			{
				f->page = p;
				p->frame = f;
				// ...
				if(++frmIndex >= frames.count()) frmIndex = 0;
			}
			else
			{
				Page* lru = list.getLeastRecentlyUsed();

				lru->frame->page = p;
				p->frame = lru->frame;
				lru->frame = NULL;
			}
		}
		list.update(p);
		listener->step(fp, p, frames, NULL);
	}
}
