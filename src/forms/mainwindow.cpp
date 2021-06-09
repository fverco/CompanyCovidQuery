#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!surveyDb.createDatabase())
        QApplication::quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

