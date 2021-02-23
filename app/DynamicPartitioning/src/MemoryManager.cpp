#include "MemoryManager.h"
#include <limits.h>

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

const char* strategyName(int strategy)
{
	switch(strategy)
	{
		case STRATEGY_FIRST_FIT:	return "STRATEGY_FIRST_FIT";
		case STRATEGY_BEST_FIT:		return "STRATEGY_BEST_FIT";
		case STRATEGY_WORST_FIT:	return "STRATEGY_WORST_FIT";
		case STRATEGY_NEXT_FIT:		return "STRATEGY_NEXT_FIT";
	}
	return "";
}

MemoryManager::MemoryManager() : head(0), tail(0), last(0), size(0)
{
}
MemoryManager::~MemoryManager()
{
	clear();
}
void MemoryManager::clear()
{
	Chunk* c;
	Chunk* n;
	for(c = head; c != NULL; c = n)
	{
		n = c->next;
		delete c;
	}
	size = 0;
}
void MemoryManager::init(int size__)
{
	clear();
#if 1
	size = size__;
	head = tail = new Chunk(0, size);
#else
	struct
	{
		int size;
		bool used;
		QString name;
	}data[] =
	{
		{ 0x1400000,	true,	"M1"},		// 20MB - usado
		{ 0x1400000,	false,	},			// 20MB - libre
		{ 0x2800000,	true,	"M2"},		// 40MB - usado
		{ 0x3C00000,	false,	},			// 60MB - libre
		{ 0x1400000,	true,	"M3"},		// 20MB - usado
		{ 0x0A00000,	false,	},			// 10MB - libre
		{ 0x3C00000,	true,	"M4"},		// 60MB - usado
		{ 0x2800000,	false,	},			// 40MB - libre
		{ 0x1400000,	true,	"M5"},		// 20MB - usado
		{ 0x1E00000,	false,	},			// 30MB - libre
		{ 0x2800000,	true,	"M6"},		// 40MB - usado
		{ 0x2800000,	false,	},			// 40MB - libre
	};
	int p = 0;
	Chunk* c = 0;
	Chunk* l = 0;
	for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
	{
		c = new Chunk(p, data[i].size);
		c->used = data[i].used;
		c->name = data[i].name;
		p += data[i].size;
		if(!l)
		{
			l = c;
			head = c;
		}
		else
		{
			l->next = c;
			c->prev = l;
			l = c;
		}
		tail = c;
	}
	size = p;
#endif
}
bool MemoryManager::free(Chunk* c)
{
	c->used = false;
	c->name = QString();

	if(c->next)
	{
		Chunk* n = c->next;
		if(!n->used)
		{
			c->size += n->size;
			if(n->next)
				n->next->prev = c;
			c->next = n->next;
			if(tail == n)
			{
				tail = c;
			}
			delete n;
		}
	}
	if(c->prev)
	{
		Chunk* p = c->prev;
		if(!p->used)
		{
			c->pos = p->pos;
			c->size += p->size;
			if(p->prev)
				p->prev->next = c;
			c->prev = p->prev;
			if(head == p)
			{
				head = c;
			}
			delete p;
		}
	}
	return true;
}
bool MemoryManager::free(const QString& name)
{
	Chunk* c;
	if(NULL != (c = find(name)))
	{
		return free(c);
	}
	return false;
}
Chunk* MemoryManager::alloc(int strategy, const QString& name, int size)
{
	Chunk* c;
	Chunk* n;

	if(NULL != (c = find(strategy, size)))
	{
		if(size == c->size)
		{
		}
		else
		{
			n = new Chunk(c->pos+size, c->size-size);

			if(c->next)
				c->next->prev = n;
			n->next = c->next;
			n->prev = c;
			c->next = n;
			n->used = false;
			c->size = size;
		}
		c->used = true;
		c->name = name;

		last = c;
		return c;
	}
	return NULL;
}
int MemoryManager::getSize() const
{
	return size;
}
Chunk* MemoryManager::getHead() const
{
	return head;
}
Chunk* MemoryManager::find(const QString& name) const
{
	for(Chunk* c = head; c != NULL; c = c->next)
	{
		if(c->used && c->name == name)
		{
			return c;
		}
	}
	return NULL;
}
Chunk* MemoryManager::find(int strategy, int size) const
{
	Chunk* c;

	switch(strategy)
	{
		case STRATEGY_FIRST_FIT:
			for(c = head; c != NULL; c = c->next)
			{
				if(!c->used && size <= c->size)
				{
					return c;
				}
			}
			break;
		case STRATEGY_BEST_FIT:
			{
				int minSize = INT_MAX;
				Chunk* minChunk = NULL;

				for(c = head; c != NULL; c = c->next)
				{
					if(!c->used && size <= c->size)
					{
						if(c->size < minSize)
						{
							minSize = c->size;
							minChunk = c;
						}
					}
				}
				return minChunk;
			}
			break;
		case STRATEGY_WORST_FIT:
			{
				int maxSize = 0;
				Chunk* maxChunk = NULL;

				for(c = head; c != NULL; c = c->next)
				{
					if(!c->used && size <= c->size)
					{
						if(c->size > maxSize)
						{
							maxSize = c->size;
							maxChunk = c;
						}
					}
				}
				return maxChunk;
			}
			break;
	}
	return NULL;
}
bool MemoryManager::load(const QString& fileName)
{
	bool ok = false;
	QFile file(fileName);
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QXmlStreamReader reader(&file);

		//
		// Parsear documento
		//
		int p = 0;
		Chunk* c = 0;
		Chunk* l = 0;
		while(!reader.atEnd() && !reader.hasError())
		{
			QXmlStreamReader::TokenType token = reader.readNext();

			if(token == QXmlStreamReader::StartDocument)
			{
			}
			else if(token == QXmlStreamReader::StartElement)
			{
				if(reader.name() == "ChunkList")
				{
				}
				else if(reader.name() == "Chunk")
				{
					QXmlStreamAttributes attrs = reader.attributes();
					QString sSize = attrs.value("size").toString();
					QString sName = attrs.value("name").toString();
					QString sColor = attrs.value("color").toString();
					bool iok = false;
					int size;

					if(sSize.size() >= 2 && sSize.at(0) == '0' && sSize.at(1).toLower() == 'x')
						size = sSize.toInt(&iok, 16);
					else
						size = sSize.toInt(&iok, 10);

					c = new Chunk(p, size);
					c->name = sName;
					if(!sColor.isEmpty())
					{
						c->color = QColor(sColor);
					}
					if(!c->name.isEmpty())
						c->used = true;
					p += c->size;
					if(!l)
					{
						l = c;
						head = c;
					}
					else
					{
						l->next = c;
						c->prev = l;
						l = c;
					}
					tail = c;
				}
			}
		}
		size = p;

		//
		// Comprobar que no haya errores
		//
		if(!reader.hasError())
		{
			ok = true;
		}
		//
		// Cerrar archivo
		//
		file.close();
	}
	return ok;
}
bool MemoryManager::save(const QString& fileName)
{
	bool ok = false;
	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly))
	{
		QXmlStreamWriter writer(&file);

		writer.setAutoFormatting(true);
		writer.writeStartDocument();

		//
		// Escribir header
		//
		writer.writeStartElement("ChunkList");

		//
		// Escribir procesos
		//
		for(Chunk* c = head; c != NULL; c = c->next)
		{
			writer.writeStartElement("Chunk");
			writer.writeAttribute("size", QString::number(c->size));
			writer.writeAttribute("name", c->name);
			if(c->color.isValid())
			{
				writer.writeAttribute("color", c->color.name());
			}
			writer.writeEndElement();
		}
		// Fin del header
		writer.writeEndElement();

#if (QT_VERSION >= 0x050000)
		if(!writer.hasError())
#else
#endif
		{
			ok = true;
		}

		//
		// Cerrar archivo
		//
		file.close();
	}
	return ok;
}


