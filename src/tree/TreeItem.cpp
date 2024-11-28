#include "TreeItem.h"

namespace {

template <template <typename T> class Container, typename T>
int findIndex(Container<std::unique_ptr<T>>& c, const T* elem) {
    auto it = std::find_if(c.begin(), c.end(), [&elem](std::unique_ptr<T>& i){
        return (i.get() == elem);
    });

    if (it == c.end()) {
        return -1;
    }
    return it - c.begin();
}

}

TreeItem::TreeItem(QVariant data)
    : m_childItems{}
    , m_itemData{data}
    , m_parentItem{nullptr}
{};

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row > (m_childItems.size() - 1)) {
        return nullptr;
    }
    return m_childItems[row].get();
}

int TreeItem::childCount() const
{
    return m_childItems.size();
}

int TreeItem::columnCount() const
{
    return 5;
}

QVariant TreeItem::data(int column) const
{
    switch (column) {
    case (0): {
        return m_itemData;
        break;
    }
    case (1):
    case (2): {
        return QVariant{"Some data"};
    }
    default: {
        return QVariant{"other"};
    }
    }
}

int TreeItem::row() const
{
    if (!m_parentItem) {
        return -1;
    }
    auto& neighbours = m_parentItem->m_childItems;
    return findIndex(neighbours, this);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

void TreeItem::addChild(std::unique_ptr<TreeItem> &&child)
{
    child->m_parentItem = this;
    m_childItems.emplace_back(std::move(child));
}

std::unique_ptr<TreeItem> TreeItem::removeChild(int index)
{
    if (index < 0 && index > (m_childItems.size() - 1)) {
        return {};
    }

    auto result = std::move(m_childItems[index]);
    m_childItems.erase(m_childItems.begin() + index);
    return result;
}

void TreeItem::move(TreeItem *newParent, int row)
{
    // beginRemoveRows(oldParent, oldRow, oldRow);
    // std::unique_ptr<TreeItem> movingItem = std::move(sourceParentItem->m_childItems[oldRow]);
    // sourceParentItem->m_childItems.erase(sourceParentItem->m_childItems.begin() + oldRow);


}

void TreeItem::print(TreeItem* item, int depth)
{
    QString indent;
    for (int i = 0; i < depth; ++i) {
        indent += "    ";
    }
    if (!item->parentItem()) {
        qDebug () << "<<Root item>>";
    } else {
        qDebug() << indent << "- " << item->m_itemData;
    }

    for (auto& child : item->m_childItems) {
        print(child.get(), depth + 1);
    }
}
