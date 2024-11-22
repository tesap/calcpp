// src/MainWindow.cpp

#include "MainWindow.h"
#include "calendar/CalendarView.h"
#include "ui_MainWindow.h"
#include "views/TreeView.h"

#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    treeView = qobject_cast<TreeViewWidget*>(ui->treeView);
    calendarView = qobject_cast<CalendarView*>(ui->calendarView);
}

void MainWindow::on_pushButton_clicked() {
    calendarView->addTask({18, 0.33, "Test Task"});
}

MainWindow::~MainWindow() {
    delete ui;
}
