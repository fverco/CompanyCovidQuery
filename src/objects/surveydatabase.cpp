#include "surveydatabase.h"

#include <QSqlDatabase>
#include <QSqlTableModel>

SurveyDatabase::SurveyDatabase(QObject *parent) :
    QObject(parent),
    surveyDb(QSharedPointer<QSqlDatabase>(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SurveyCon")))),
    surveyModel(QSharedPointer<QSqlTableModel>(new QSqlTableModel(this, *surveyDb))),
    dbLocation("")
{
}

void SurveyDatabase::createDatabase()
{

}

void SurveyDatabase::createTableModel()
{

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
