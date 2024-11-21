// include/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class TreeViewWidget;
// class ListViewWidget;
class CalendarView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    TreeViewWidget *treeView;
    // ListViewWidget *listView;
    CalendarView*calendarView;
};

#endif // MAINWINDOW_H

