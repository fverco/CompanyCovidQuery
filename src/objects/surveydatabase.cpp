#include "surveydatabase.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QFile>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>

SurveyDatabase::SurveyDatabase(QObject *parent) :
    QObject(parent),
    surveyDb(QSharedPointer<QSqlDatabase>(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SurveyCon")))),
    surveyModel(QSharedPointer<QSqlTableModel>(new QSqlTableModel(this, *surveyDb))),
    dbLocation("")
{
}

bool SurveyDatabase::createDatabase(const QString &dir)
{
    dbLocation = dir;

    if (!QFile::exists(dir)) {
        surveyDb->setDatabaseName(dir);

        if (surveyDb->open()) {
            QSqlQuery surveyQry(*surveyDb);

            surveyQry.prepare("CREATE TABLE Employee ("
                              "emp_id INTEGER UNIQUE NOT NULL PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL);");

            if (!surveyQry.exec()) {
                qDebug() << "(DB) Error creating employee info: " << surveyQry.lastError().text() << Qt::endl;
                closeDb();
                return false;
            }

            surveyQry.prepare("CREATE TABLE Survey ("
                              "survey_date INTEGER NOT NULL,"
                              "emp_id INTEGER NOT NULL,"
                              "q_one INTEGER,"
                              "q_two INTEGER,"
                              "q_three INTEGER,"
                              "temperature REAL,"
                              "PRIMARY KEY(survey_date, emp_id),"
                              "FOREIGN KEY(emp_id) REFERENCES Employee(emp_id)"
                              ");");

            if (!surveyQry.exec()) {
                qDebug() << "(DB) Error creating survey info: " << surveyQry.lastError().text() << Qt::endl;
                closeDb();
                return false;
            } else
                createTableModel();
        }
    }

    return true;
}

void SurveyDatabase::createTableModel()
{
    openDb();

    surveyModel->setTable(tr("Surveys"));
    surveyModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    surveyModel->select();
    surveyModel->setHeaderData(0, Qt::Horizontal, tr("Survey Date"));
    surveyModel->setHeaderData(1, Qt::Horizontal, tr("Question 1"));
    surveyModel->setHeaderData(2, Qt::Horizontal, tr("Question 2"));
    surveyModel->setHeaderData(3, Qt::Horizontal, tr("Question 3"));
    surveyModel->setHeaderData(4, Qt::Horizontal, tr("Temperature"));

    closeDb();
}

void SurveyDatabase::openDb()
{
    if (!surveyDb->isOpen()) {
        surveyDb->setDatabaseName(dbLocation);
        surveyDb->open();
    }
}

void SurveyDatabase::closeDb()
{
    if (surveyDb->isOpen())
        surveyDb->close();
}
