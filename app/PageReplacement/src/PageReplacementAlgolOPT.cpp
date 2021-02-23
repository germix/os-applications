#include "PageReplacementAlgolOPT.h"

#include <QVector>
#include "PageListLRU.h"
#include "PageProvider.h"
#include "PageReferences.h"
#include "FrameProvider.h"
#include "PageReplacementExecutorListener.h"

QString PageReplacementAlgolOPT::name() const
{
	return "OPTIMO";
}
void    PageReplacementAlgolOPT::exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames)
{
	bool fp;
	int frmIndex = 0;

	listener->init(frames, references);
	for(int s = 0; s < references.count(); s++)
	{
		Page* p = pages.findPage(references[s], &fp);

		if(fp)
		{
			Frame* m = frames[frmIndex];

			if(m->page == NULL)	// ¿Equivale a p.marco == null?
			{
				m->page = p;
				p->frame = m;
				// ...
				if(++frmIndex >= frames.count()) frmIndex = 0;
			}
			else
			{
				/*
				ListaPaginasLRU lista = new ListaPaginasLRU();
				for(int i = 0; i < marcos.length; i++)
					lista.actualizar(marcos[i].pagina);

				for(int t = s+1; t < secuencia.length; t++)
				{
					Pagina x = Utils.paginaDesdeNumero(paginas, secuencia[t]);

					// Actualizar si está referenciada por algun marco
					for(int i = 0; i < marcos.length; i++)
					{
						if(marcos[i].pagina == x)
						{
							lista.actualizar(x);
						}
					}

				}
				Pagina lru = lista.laUltimaReferenciada();
				*/
				Page* lru = NULL;
				QVector<Page*> paginasEnMarcos;

				for(int i = 0; i < frames.count(); i++)
					paginasEnMarcos.append(frames[i]->page);

				if(true)
				{
					for(int t = s+1; t < references.count(); t++)
					{
						Page* x = pages.findPage(references[t]);
						int indexOf = paginasEnMarcos.indexOf(x);
						if(indexOf != -1)
						{
							paginasEnMarcos.remove(indexOf);
						}
						if(paginasEnMarcos.size() == 1)
						{
							break;
						}
					}
					lru = paginasEnMarcos.at(0);
					lru->frame->page = p;
					p->frame = lru->frame;
					lru->frame = NULL;
				}
				else
				{
					for(int t = s+1; t < references.count(); t++)
					{
						Page* x = pages.findPage(references[t]);
						int indexOf = paginasEnMarcos.indexOf(x);
						if(indexOf != -1)
						{
							paginasEnMarcos.remove(indexOf);
						}
					}
					if(paginasEnMarcos.isEmpty())
					{
						lru = frames[frmIndex]->page;
						lru->frame->page = p;
						p->frame = lru->frame;
						lru->frame = NULL;
					}
					else
					{
						lru = paginasEnMarcos.at(0);
						lru->frame->page = p;
						p->frame = lru->frame;
						lru->frame = NULL;
					}
				}

				if(++frmIndex >= frames.count()) frmIndex = 0;
			}
		}
		listener->step(fp, p, frames, NULL);
	}
}

