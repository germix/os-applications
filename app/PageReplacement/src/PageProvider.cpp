#include "PageProvider.h"
#include <stdint.h>
#include <qglobal.h>

#include "Page.h"
#include "PageReferences.h"

#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif

PageProvider::PageProvider(PageReferences& references) : pages(0), count(0)
{
	//
	// Generar p�ginas
	//
	if(references.count() > 0)
	{
		int max = 0;
		int min = INT32_MAX;
		for(int s = 0; s < references.count(); s++)
		{
			max = qMax(max, references[s]);
			min = qMin(min, references[s]);
		}
		count = (max-min)+1;
		pages = new Page[count];
		for(int i = 0; i < count; i++)
		{
			pages[i] = Page(i+min);
		}
	}
}
PageProvider::~PageProvider()
{
	//
	// Eliminar p�ginas
	//
	if(pages)
		delete [] pages;
}
Page* PageProvider::findPage(int num)
{
	return findPage(num, NULL);
}
Page* PageProvider::findPage(int num, bool* pageFault)
{
	if(pageFault) *pageFault = false;

	for(int i = 0; i < count; i++)
	{
		if(pages[i].num == num)
		{
			// Comprobar si la p�gina est� asignada a algun marco, si no lo est� generar fallo de p�gina
			if(pages[i].frame == NULL)
			{
				if(pageFault) *pageFault = true;
			}
			// Retornar p�gina
			return &pages[i];
		}
	}
	return NULL;
}
