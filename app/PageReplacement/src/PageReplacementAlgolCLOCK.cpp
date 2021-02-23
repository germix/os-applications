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
				// Sistemas Operatvos 4� Edici�n - William Stallings - P�gina 346
				//
				// Cuando se carga una p�gina por primera vez en un marco de memoria, el bit de uso de dicho marco se
				// pone a cero. Cuando se hace referencia a la p�gina posteriormente (despu�s de la referencia que
				// gener� el fallo de p�gina), el bit de uso se pone a 1. Para el algoritmo de reemplazo de p�ginas,
				// el conjunto de marcos candidatos a ser reemplazados se considera como un buffer circular con un
				// puntero asociado. Al reemplazar una p�gina, se hace que el puntero se�ale al siguiente marco
				// del buffer. Cuando llega el momento de reemplazar una p�gina, el sistema operativo recorre el buffer
				// buscando un marco con el bit de uso a 0. Cada vez que se encuentra un arco con el bit de uso a 1,
				// lo pone a 0. Si alg�n marco del buffer tiene el bit de uso a 0 al comienzo de la b�squeda, se elige para
				// reemplazar el primero que se haya encontrado. Si todos los marcos tienen el bit de uso puesto a 1, el
				// puntero dar� una vuelta completa al buffer, poniendo todos los bits a 0 y se detendr� en la posici�n inicial,
				// reemplazando la p�gina de dicho marco.
				//
				// --------------------------------------------------------------------------------------------------------
				//
				// Sistemas Operativos Modernos 3� Edici�n - Andrew S. Tenenbaum - P�ginas 205-206
				//
				// Cuando ocurre un fallo de p�gina, la p�gina a la que apunta la manecilla se inspecciona. Si el bit
				// R es 0, la p�gina se desaloja, se inserta la nueva p�gina en el reloj en su lugar y la manecilla se avanza
				// una posici�n. Si R es 1, se borra y la manecilla se avanza a la siguiente p�gina. Este proceso se repite
				// hasta encontrar una p�gina con R = 0. No es de sorprender que a este algoritmo se le llame en reloj.
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
