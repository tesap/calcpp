
#include "TreeModel.h"

#include <QIODevice>


TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel{parent}
    , rootItem{std::make_unique<TreeItem>("<<ROOT ITEM>>")}
{}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    TreeItem *parentItem = getItem(parent);
    return index(row, column, parentItem);
}

QModelIndex TreeModel::index(int row, int column, TreeItem *parent) const
{
    if (auto *childItem = parent->child(row)) {
        return std::move(createIndex(row, column, childItem));
    }
    return {};
}


QModelIndex TreeModel::index(TreeItem *item) const
{
    int itemRow = item->row();
    if (itemRow == -1) {
        return {};
    }
    return index(itemRow, 0, item);
}


QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    auto indexItem = getItem(index);
    if (!index.isValid()) {
        return {};
    }

    auto parentItem = indexItem->parentItem();

    if (parentItem == rootItem.get()) {
        return QModelIndex{};
    } else {
        auto result = createIndex(parentItem->row(), 0, parentItem);
        return result;
    }
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    const TreeItem *parentItem = getItem(parent);
    auto result = parentItem->childCount();
    return result;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return getItem(parent)->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    auto item = getItem(index);
    auto result = item->data(index.column());
    return result;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    bool flag = orientation == Qt::Horizontal && role == Qt::DisplayRole;

    if (!flag) {
        return {};
    }
    return rootItem->data(section);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemFlags(Qt::NoItemFlags);
    }

    auto item = getItem(index);
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    // Allow dragging leaves only
    if (item->childCount() == 0) {
        defaultFlags |= Qt::ItemIsDragEnabled;
    }

    // Allow dropping only on non-leaf items
    if (item->childCount() > 0) {
        defaultFlags |= Qt::ItemIsDropEnabled;
    }

    return defaultFlags;
}

QStringList TreeModel::mimeTypes() const
{
    return { "application/x-treeitem" };
}


QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    QMimeData *mimeData = new QMimeData;

    // Serialize the item
    const QModelIndex index = indexes.first();
    if (index.isValid()) {
        QByteArray encodedData;
        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        TreeItem *item = getItem(index);
        stream << item->data(0).toString();                         // Example: Serialize the item name
        stream << index.row();                                      // Serialize row
        stream << reinterpret_cast<quintptr>(item);    // Serialize item's ptr

        mimeData->setData("application/x-treeitem", encodedData);
    }

    return mimeData;
}


bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                             int row, int column, const QModelIndex &parent)
{
    if (!data || action != Qt::MoveAction) {
        return false;
    }

    if (!data->hasFormat("application/x-treeitem")) {
        return false;
    }

    QByteArray encodedData = data->data("application/x-treeitem");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QString itemName;
    int oldRow;
    quintptr itemPtr;
    TreeItem* movingItem;

    // Deserialize data
    stream >> itemName >> oldRow >> itemPtr;
    movingItem = reinterpret_cast<TreeItem* >(itemPtr);

    TreeItem *oldParent = movingItem->parentItem();
    const QModelIndex oldParentIndex = index(oldParent);
    TreeItem *newParent = getItem(parent);

    if (!movingItem || !newParent) {
        return false;
    }

    // Ensure that tasks (leaves) can only be dropped into projects (non-leaves)
    if (movingItem->childCount() != 0 || newParent->childCount() == 0) {
        return false;
    }

    // Move the item
    beginRemoveRows(oldParentIndex, oldRow, oldRow);
    auto movingItemPtr = oldParent->removeChild(oldRow);
    // sourceItem->move(targetParentItem, )
    // std::unique_ptr<TreeItem> movingItem = std::move(sourceParentItem->m_childItems[oldRow]);
    // sourceParentItem->m_childItems.erase(sourceParentItem->m_childItems.begin() + oldRow);
    endRemoveRows();

    beginInsertRows(parent, row, row);
    newParent->addChild(std::move(movingItemPtr));
    // targetParentItem->addChild(std::move(movingItem));
    endInsertRows();

    return true;
}


Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::MoveAction;
}



void TreeModel::addItem(const QString &name, const QModelIndex &parent)
{
    TreeItem* parentItem = getItem(parent);

    beginInsertRows(parent, parentItem->childCount(), parentItem->childCount());

    auto newItem = std::make_unique<TreeItem>(name);
    parentItem->addChild(std::move(newItem));

    endInsertRows();
}

TreeItem* TreeModel::getItem(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return rootItem.get();
    }
    return static_cast<TreeItem*>(index.internalPointer());
}
