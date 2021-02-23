#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include <QString>
#include <QColor>

enum
{
	STRATEGY_FIRST_FIT,		// Primer ajuste
	STRATEGY_BEST_FIT,		// Mejor ajuste
	STRATEGY_WORST_FIT,		// Peor ajuste
	STRATEGY_NEXT_FIT,		// Siguiente ajuste
};

class Chunk
{
public:
	Chunk*	next;
	Chunk*	prev;
	int		pos;
	int		size;
	bool	used;
	QString	name;
	QColor	color;

	Chunk(int pos, int size)
	{
		this->next = 0;
		this->prev = 0;
		this->pos = pos;
		this->size = size;
		this->used = false;
	}
};

class MemoryManager
{
	Chunk* head;
	Chunk* tail;
	Chunk* last;

	int		size;
public:
	MemoryManager();
	~MemoryManager();
public:
	void clear();
	void init(int size);
	bool free(Chunk* c);
	bool free(const QString& name);
	Chunk* alloc(int strategy, const QString& name, int size);

	int getSize() const;
	Chunk* getHead() const;

	Chunk* find(const QString& name) const;
	Chunk* find(int strategy, int size) const;

	bool load(const QString& fileName);
	bool save(const QString& fileName);
};

#endif // MEMORYMANAGER_H
