#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMimeData>
#include <QDrag>
#include <QDebug>

class CustomStandardItem : public QStandardItem {
public:
    CustomStandardItem(const QString &text) : QStandardItem(text) {}

    // Store nesting level in custom role
    void setNestingLevel(int level) {
        setData(level, Qt::UserRole);
    }

    int getNestingLevel() const {
        return data(Qt::UserRole).toInt();
    }
};

class CustomModel : public QStandardItemModel {
public:
    CustomModel(QObject *parent = nullptr) : QStandardItemModel(parent) {}

protected:
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override {
        // Get the target item where the drop is happening
        QStandardItem *targetItem = itemFromIndex(parent);

        // Get the source item from the mime data (the dragged item)
        QModelIndex sourceIndex = data->property("sourceIndex").value<QModelIndex>();
        QStandardItem *sourceItem = itemFromIndex(sourceIndex);

        // Cast to CustomStandardItem to access nesting level
        CustomStandardItem *sourceCustomItem = static_cast<CustomStandardItem *>(sourceItem);
        CustomStandardItem *targetCustomItem = static_cast<CustomStandardItem *>(targetItem);

        if (!sourceCustomItem || !targetCustomItem) {
            qDebug() << "Invalid cast, something went wrong with source or target item!";
            return false;
        }

        // Compare the nesting levels of the source and target items
        if (sourceCustomItem->getNestingLevel() != targetCustomItem->getNestingLevel()) {
            // If they are not at the same level, reject the drop
            qDebug() << "Drop rejected: items are at different nesting levels.";
            return false;
        }

        // Otherwise, accept the drop
        return QStandardItemModel::dropMimeData(data, action, row, column, parent);
    }
};

void buildTree(QStandardItem *parentItem, int currentLevel) {
    // Add some child items with levels
    for (int i = 0; i < 3; ++i) {
        CustomStandardItem *item = new CustomStandardItem(QString("Task %1").arg(i+1));
        item->setNestingLevel(currentLevel);

        parentItem->appendRow(item);

        // Recursively add children with incremented nesting level
        buildTree(item, currentLevel + 1);
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CustomModel model;
    model.setHorizontalHeaderLabels({"Tasks"});

    CustomStandardItem *rootItem = new CustomStandardItem("Root");
    model.appendRow(rootItem);

    // Build the tree with precomputed nesting levels
    buildTree(rootItem, 0);

    // Set up the QTreeView
    QTreeView treeView;
    treeView.setModel(&model);
    treeView.setDragDropMode(QTreeView::InternalMove);
    treeView.setAcceptDrops(true);
    treeView.setDragEnabled(true);
    treeView.setDropIndicatorShown(true);
    treeView.show();

    return app.exec();
}
