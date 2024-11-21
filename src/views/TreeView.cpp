// src/treeviewwidget.cpp
#include "TreeView.h"
#include <QTreeView>
#include <QVBoxLayout>

TreeViewWidget::TreeViewWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    treeView = new QTreeView(this);
    layout->addWidget(treeView);

    setLayout(layout);
}

