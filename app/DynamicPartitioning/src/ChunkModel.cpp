#include "ChunkModel.h"
#include "MemoryManager.h"

QString formatBytesToSize(float bytes);

#define SHOW_COLOR 1
/*
#define SHOW_COLOR 0
#define SHOW_COLOR 1
#define SHOW_COLOR 2
*/

ChunkModel::ChunkModel(MemoryManager* mmgr__) : mmgr(mmgr__)
{
}
ChunkModel::~ChunkModel()
{
}
QModelIndex ChunkModel::index(int row, int column, const QModelIndex &parent) const
{
	int i = 0;
	Chunk* c;

	if(!parent.isValid())
	{
		for(c = mmgr->getHead(); c != NULL; c = c->next)
		{
			if(c->used)
			{
				if(i == row)
				{
					return createIndex(row, column, c);
				}
				i++;
			}
		}
	}
	return QModelIndex();
}
QModelIndex ChunkModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}
int ChunkModel::rowCount(const QModelIndex &parent) const
{
	if(parent.isValid())
	{
		return 0;
	}
	int r = 0;
	Chunk* c;
	for(c = mmgr->getHead(); c != NULL; c = c->next)
	{
		if(c->used) r++;
	}
	return r;
}
int ChunkModel::columnCount(const QModelIndex &parent) const
{
#if SHOW_COLOR==1
	return 3;
#else
	return 2;
#endif
}
QVariant ChunkModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		Chunk* chunk = ((Chunk*)index.internalPointer());

		switch(index.column())
		{
			case 0:	return chunk->name;
			case 1:	return formatBytesToSize(chunk->size);
		}
	}
#if SHOW_COLOR == 1
	else if(role == Qt::DecorationRole)
	{
		if(index.column() == 2)
		{
			Chunk* chunk = ((Chunk*)index.internalPointer());
			if(chunk->color.isValid())
			{
				return chunk->color;
			}
		}
	}
#elif SHOW_COLOR == 2
	else if(role == Qt::DecorationRole)
	{
		if(index.column() == 0)
		{
			Chunk* chunk = ((Chunk*)index.internalPointer());
			if(chunk->color.isValid())
			{
				return chunk->color;
			}
			else
			{
				return QColor(0, 0, 0, 0);
			}
		}
	}
#endif
	return QVariant();
}
QVariant ChunkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role == Qt::DisplayRole)
	{
		switch(section)
		{
			case 0:	return "Nombre";
			case 1:	return "Tamaño";
#ifdef SHOW_COLOR
			case 2:	return "Color";
#endif
		}
	}
	return QVariant();
}
bool ChunkModel::free(const QString& name)
{
	beginResetModel();
	bool ret = mmgr->free(name);
	endResetModel();
	return ret;
}
Chunk* ChunkModel::alloc(int strategy, int size, const QString& name, const QColor& color)
{
	beginResetModel();
	Chunk* chunk = mmgr->alloc(strategy, name, size);
	if(chunk != NULL)
	{
		chunk->color = color;
	}
	endResetModel();
	return chunk;
}
void ChunkModel::init(int size)
{
	beginResetModel();
	mmgr->init(size);
	endResetModel();
}
bool ChunkModel::load(const QString& fileName)
{
	beginResetModel();
	bool ok = mmgr->load(fileName);
	endResetModel();
	return ok;
}
bool ChunkModel::save(const QString& fileName)
{
	return mmgr->save(fileName);
}
