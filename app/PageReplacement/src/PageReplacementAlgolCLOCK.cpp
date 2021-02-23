#include "PageReplacementAlgolCLOCK.h"

#include "PageProvider.h"
#include "PageReferences.h"
#include "PageReplacementExecutorListener.h"
#include "FrameProvider.h"

QString PageReplacementAlgolCLOCK::name() const
{
	return "CLOCK";
}
void    PageReplacementAlgolCLOCK::exec(PageReplacementExecutorListener* listener, PageReferences& references, PageProvider& pages, FrameProvider& frames)
{
	bool fp;
	int frmIndex = 0;
	//int mPuntero = 0;

	bool primerBitUno = true;

	listener->init(frames, references);
	for(int s = 0; s < references.count(); s++)
	{
		Page* p = pages.findPage(references[s], &fp);

		if(p->refbit.isInvalid())
		{
			p->refbit = 0;
		}
		if(fp)
		{
			Frame* m = frames[frmIndex];

			if(m->page == NULL)
			{
				if(primerBitUno)
					p->refbit = 1;
				else
					p->refbit = 0;

				m->page = p;
				p->frame = m;
				if(++frmIndex >= frames.count()) frmIndex = 0;
			}
			else
			{
				//
				// Sistemas Operatvos 4º Edición - William Stallings - Página 346
				//
				// Cuando se carga una página por primera vez en un marco de memoria, el bit de uso de dicho marco se
				// pone a cero. Cuando se hace referencia a la página posteriormente (después de la referencia que
				// generó el fallo de página), el bit de uso se pone a 1. Para el algoritmo de reemplazo de páginas,
				// el conjunto de marcos candidatos a ser reemplazados se considera como un buffer circular con un
				// puntero asociado. Al reemplazar una página, se hace que el puntero señale al siguiente marco
				// del buffer. Cuando llega el momento de reemplazar una página, el sistema operativo recorre el buffer
				// buscando un marco con el bit de uso a 0. Cada vez que se encuentra un arco con el bit de uso a 1,
				// lo pone a 0. Si algún marco del buffer tiene el bit de uso a 0 al comienzo de la búsqueda, se elige para
				// reemplazar el primero que se haya encontrado. Si todos los marcos tienen el bit de uso puesto a 1, el
				// puntero dará una vuelta completa al buffer, poniendo todos los bits a 0 y se detendrá en la posición inicial,
				// reemplazando la página de dicho marco.
				//
				// --------------------------------------------------------------------------------------------------------
				//
				// Sistemas Operativos Modernos 3º Edición - Andrew S. Tenenbaum - Páginas 205-206
				//
				// Cuando ocurre un fallo de página, la página a la que apunta la manecilla se inspecciona. Si el bit
				// R es 0, la página se desaloja, se inserta la nueva página en el reloj en su lugar y la manecilla se avanza
				// una posición. Si R es 1, se borra y la manecilla se avanza a la siguiente página. Este proceso se repite
				// hasta encontrar una página con R = 0. No es de sorprender que a este algoritmo se le llame en reloj.
				//
				while(true)
				{
					m = frames[frmIndex];
					if(m->page->refbit == 1)
					{
						//
						// Ponemos en 0 y le damos otra oportunidad
						//
						m->page->refbit = 0;
						if(++frmIndex >= frames.count()) frmIndex = 0;
					}
					else // if(m.pagina.bitUso == 0)
					{
						if(primerBitUno)
							p->refbit = 1;
						else
							p->refbit = 0;

						if(m->page != NULL)
							m->page->frame = NULL;
						m->page = p;
						p->frame = m;
						if(++frmIndex >= frames.count()) frmIndex = 0;
						break;
					}
				}
			}
		}
		else
		{
			p->refbit = 1;
		}
		listener->step(fp, p, frames, NULL);
	}
}
