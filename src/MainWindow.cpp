// src/MainWindow.cpp

#include "MainWindow.h"
#include "views/CalendarView.h"
#include "views/TreeView.h"

#include <QSplitter>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Splitter to hold the task views and calendar view
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Tree View
    treeView = new TreeViewWidget(this);
    splitter->addWidget(treeView);

    // List View
    // listView = new ListViewWidget(this);
    // splitter->addWidget(listView);

    // Calendar View
    calendarView = new CalendarView(this);
    splitter->addWidget(calendarView);

    // Set central widget
    setCentralWidget(splitter);

    // Window properties
    setWindowTitle("Task Planner");
    resize(1200, 800);
}

MainWindow::~MainWindow() = default;

