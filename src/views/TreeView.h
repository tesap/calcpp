// include/treeviewwidget.h
#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QTreeView>

class TreeViewWidget : public QTreeView {
    Q_OBJECT

public:
    explicit TreeViewWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // TREEVIEWWIDGET_H
