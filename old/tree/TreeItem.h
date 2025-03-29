

#include <memory>
#include <vector>
#include <QVariant>

#pragma once


//  - Proj1
//      - Sub-proj 1.1
//          - Task 1
//          - Task 2
//  - Proj2
//      - Task 1


class TreeItem
{
public:
    explicit TreeItem(QVariant data);

    // Read-only
    TreeItem *child(int row);           // OK
    int childCount() const;             // OK
    int columnCount() const;            // OK
    QVariant data(int column) const;    // OK (Placeholder)
    int row() const;                    // OK (O(n) calc of parent.child_list)
    TreeItem *parentItem();             // OK

    // Write (Add/Remove)
    void addChild(std::unique_ptr<TreeItem> &&child);    // .
    std::unique_ptr<TreeItem> removeChild(int index);
    void move(TreeItem* newParent, int row);

    // Print
    static void print(TreeItem* item, int depth = 0);

private:
    // Hierarchy info
    TreeItem *m_parentItem;
    std::vector<std::unique_ptr<TreeItem>> m_childItems;

    QVariant m_itemData;
};
