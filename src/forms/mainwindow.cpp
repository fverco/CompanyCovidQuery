#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlTableModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      surveyDb()
{
    ui->setupUi(this);

    if (!surveyDb.createDatabase())
        QApplication::quit();

    ui->tableSurveys->setModel(surveyDb.getSurveyModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}

