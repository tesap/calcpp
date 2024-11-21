// include/treeviewwidget.h
#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QWidget>

class QTreeView;

class TreeViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *parent = nullptr);

private:
    QTreeView *treeView;
};

#endif // TREEVIEWWIDGET_H

