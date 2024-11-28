// src/MainWindow.cpp

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "tree/TreeModel.h"

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>
#include <QScreen>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto model = new TreeModel();

    // Inflating treeView
    model->addItem("proj (1)");
    model->addItem("proj (2)");
    auto proj1 = model->index(0, 0, QModelIndex{});
    model->addItem("proj (1.1)", proj1);
    auto proj11 = model->index(0, 0, proj1);
    auto proj2 = model->index(1, 0, QModelIndex{});

    model->addItem("Task (1).1", proj1);
    model->addItem("Task (1).2", proj1);

    model->addItem("Task (1.1).1", proj11);
    model->addItem("Task (1.1).2", proj11);


    model->addItem("Task (2).1", proj2);
    model->addItem("Task (2).2", proj2);


    TreeItem::print(model->rootItem.get());

    treeView = qobject_cast<QTreeView*>(ui->treeView);
    treeView->setModel(model);
    treeView->setWindowTitle("Task Tree View");

    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);

    treeView->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}

