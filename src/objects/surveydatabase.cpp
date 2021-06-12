#include "surveydatabase.h"

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QFile>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>

/*!
 * \brief The constructor for the SurveyDatabase.
 * \param parent = The QObject to which this object is bound to.
 * The currentEmpId member is initialized to 1. This is because it is the number with which SQLite starts with.
 */
SurveyDatabase::SurveyDatabase(QObject *parent) :
    QObject(parent),
    surveyDb(QSharedPointer<QSqlDatabase>(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SurveyCon")))),
    surveyModel(QSharedPointer<QSqlQueryModel>(new QSqlQueryModel(this))),
    dbLocation(""),
    currentEmpId(1)
{
}

/*!
 * \brief Creates the database file if it doesn't exist and updates the survey model.
 * \param dir = The full path to where the database file should be stored
 * \return A boolean value stating whether the creation was successful or not.
 */
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
            }
        }
    }

    updateTableModel();

    return true;
}

/*!
 * \brief Returns a pointer to the DB's survey model.
 * \return A QSqlQueryModel pointer of the model.
 * \note The returned pointer MUST NOT be deleted.
 */
QSqlQueryModel *SurveyDatabase::getSurveyModel()
{
    return surveyModel.data();
}

/*!
 * \brief Update the survey model with the current employee ID.
 *
 * Updates the survey model to display the current state of survey data from the current employee ID from currentEmpId.
 */
void SurveyDatabase::updateTableModel()
{
    openDb();

    surveyModel->setQuery("SELECT survey_date, q_one, q_two, q_three, temperature "
                          "FROM Survey "
                          "WHERE emp_id = " + QString::number(currentEmpId) + " "
                          "ORDER BY survey_date;", *surveyDb);

    surveyModel->setHeaderData(0, Qt::Horizontal, tr("Survey Date"));
    surveyModel->setHeaderData(1, Qt::Horizontal, tr("Question 1"));
    surveyModel->setHeaderData(2, Qt::Horizontal, tr("Question 2"));
    surveyModel->setHeaderData(3, Qt::Horizontal, tr("Question 3"));
    surveyModel->setHeaderData(4, Qt::Horizontal, tr("Temperature"));

    closeDb();
}

/*!
 * \brief Opens a connection to the database.
 */
void SurveyDatabase::openDb()
{
    if (!surveyDb->isOpen()) {
        surveyDb->setDatabaseName(dbLocation);
        surveyDb->open();
    }
}

/*!
 * \brief Closes the connection to the database if it is open.
 */
void SurveyDatabase::closeDb()
{
    if (surveyDb->isOpen())
        surveyDb->close();
}
