// src/treeviewwidget.cpp
#include "TreeView.h"
#include <QMouseEvent>
#include <QDebug>

TreeViewWidget::TreeViewWidget(QWidget *parent) : QTreeView(parent) {
    setSelectionMode(QAbstractItemView::SingleSelection); // Example customization
}

void TreeViewWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Mouse pressed on TreeView!";
    QTreeView::mousePressEvent(event); // Call base class behavior
}
