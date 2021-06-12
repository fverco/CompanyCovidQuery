#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlTableModel>

/*!
 * \brief The constructor for the MainWindow.
 * \param parent = The QWidget to which this window is bound to.
 */
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

/*!
 * \brief The destructor for the MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

