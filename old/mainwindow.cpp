#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_triggered(QAction *arg1)
{
    std::cout << "0000\n";
}


void MainWindow::on_pushButton_clicked()
{
    std::cout << "1111\n";
}

