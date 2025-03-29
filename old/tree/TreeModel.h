
#include <QAbstractItemModel>
#include <QMimeData>

#include "TreeItem.h"

#pragma once


class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    Q_DISABLE_COPY_MOVE(TreeModel)

    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel() override = default;

    // Read-only
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;

    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // External write
    void addItem(const QString& name, const QModelIndex& parent = QModelIndex());

    // DragnDrop
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QStringList mimeTypes() const override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    std::unique_ptr<TreeItem> rootItem;
private:
    QModelIndex index(int row, int column, TreeItem* parent = nullptr) const;
    QModelIndex index(TreeItem* item) const;

    TreeItem* getItem(const QModelIndex& index) const;
    static void setupModelData(const QList<QStringView> &lines, TreeItem *parent);
};
