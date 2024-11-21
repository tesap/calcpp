// src/MainWindow.cpp

#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "views/CalendarView.h"
#include "views/TreeView.h"

#include <QSplitter>
#include <QVBoxLayout>

/*MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {*/
/*    // Splitter to hold the task views and calendar view*/
/*    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);*/
/**/
/*    // Tree View*/
/*    treeView = new TreeViewWidget(this);*/
/*    splitter->addWidget(treeView);*/
/**/
/*    // List View*/
/*    // listView = new ListViewWidget(this);*/
/*    // splitter->addWidget(listView);*/
/**/
/*    // Calendar View*/
/*    calendarView = new CalendarView(this);*/
/*    splitter->addWidget(calendarView);*/
/**/
/*    // Set central widget*/
/*    setCentralWidget(splitter);*/
/**/
/*    // Window properties*/
/*    setWindowTitle("Task Planner");*/
/*    resize(1200, 800);*/
/*}*/
/**/
/*MainWindow::~MainWindow() = default;*/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Access promoted widgets
    auto *treeView = qobject_cast<TreeViewWidget *>(ui->treeView);
    auto *calendar = qobject_cast<CalendarView *>(ui->calendarView);

    /*if (treeView) {*/
    /*    // Setup the task tree view*/
    /*    treeView->setModel(new QStandardItemModel(this)); // Example*/
    /*}*/
    /**/
    /*if (calendar) {*/
    /*    // Initialize the calendar view*/
    /*    calendar->initialize();*/
    /*}*/
}

MainWindow::~MainWindow() {
    delete ui;
}

