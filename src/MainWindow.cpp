// src/MainWindow.cpp

#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "views/CalendarView.h"
#include "views/TreeView.h"

#include <QSplitter>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    treeView = qobject_cast<TreeViewWidget *>(ui->treeView);
    calendarView = qobject_cast<CalendarView *>(ui->calendarView);
}

void MainWindow::on_pushButton_clicked() {
    Task task;
    task.name = "Test Task";
    task.startHour = 18;
    task.duration = 0.33;
    task.color = QColor::fromHsv(200, 255, 200);

    calendarView->addTask(task);
}

MainWindow::~MainWindow() {
    delete ui;
}

