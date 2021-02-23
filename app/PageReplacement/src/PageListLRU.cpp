#include "PageListLRU.h"

#ifndef NULL
#define NULL 0
#endif

PageListLRU::PageListLRU() : head(0), tail(0)
{
}
PageListLRU::~PageListLRU()
{
	clear();
}
void PageListLRU::clear()
{
	PageNodeLRU* p;
	PageNodeLRU* n;
	for(p = head; p != NULL; p = n)
	{
		n = p->next;
		delete p;
	}
	head = NULL;
	tail = NULL;
}
void PageListLRU::update(Page* p)
{
	PageNodeLRU* match = NULL;

	// Buscar si la página 'p' está  en la lista de LRU
	for(PageNodeLRU* n = head; n != NULL && match == NULL; n = n->next)
	{
		if(n->page == p) match = n;
	}
	if(match == NULL)
	{
		//
		// Crear nodo
		//
		match = new PageNodeLRU(p);
	}
	else
	{
		//
		// Desenlazar el nodo
		//
		if(match->prev != NULL)
			match->prev->next = match->next;
		if(match->next != NULL)
			match->next->prev = match->prev;
		if(head == match)
			head = match->next;
		if(tail == match)
			tail = match->prev;
		match->prev = NULL;
		match->next = NULL;
	}
	//
	// Agregarlo al inicio
	//
	if(head == NULL)
	{
		head = match;
		tail = match;
	}
	else
	{
		match->next = head;
		head->prev = match;
		head = match;
	}
}
Page* PageListLRU::getLeastRecentlyUsed()
{
	PageNodeLRU* n = tail;

	if(tail->prev != NULL)
		tail->prev->next = NULL;
	tail = tail->prev;

	if(tail == NULL)
	{
		head = NULL;
	}
	return n->page;
}
