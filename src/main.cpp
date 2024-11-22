//#include "CalendarView.h"
#include "MainWindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

