// include/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TreeViewWidget;
// class ListViewWidget;
class CalendarView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    TreeViewWidget *treeView;
    // ListViewWidget *listView;
    CalendarView*calendarView;
};

#endif // MAINWINDOW_H

