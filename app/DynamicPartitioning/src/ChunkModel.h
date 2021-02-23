#ifndef CHUNKMODEL_H
#define CHUNKMODEL_H
#include <QAbstractItemModel>

class Chunk;
class MemoryManager;

class ChunkModel : public QAbstractItemModel
{
	MemoryManager* mmgr;
public:
	ChunkModel(MemoryManager* mmgr);
	~ChunkModel();
public:
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
	void beginResetModel()		{ QAbstractItemModel::beginResetModel(); 	}
	void endResetModel()		{ QAbstractItemModel::endResetModel();		}
public:
	bool free(const QString& name);
	Chunk* alloc(int strategy, int size, const QString& name, const QColor& color);
	void init(int size);
	bool load(const QString& fileName);
	bool save(const QString& fileName);
};

#endif // CHUNKMODEL_H
