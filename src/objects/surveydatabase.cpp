#include "surveydatabase.h"

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QFile>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>
#include <QDateTime>
#include <QDate>

/*!
 * \brief The constructor for the SurveyDatabase.
 * \param parent = The QObject to which this object is bound to.
 * The currentEmpId member is initialized to 1. This is because it is the number with which SQLite starts with.
 */
SurveyDatabase::SurveyDatabase(QObject *parent) :
    QObject(parent),
    surveyDb(QSharedPointer<QSqlDatabase>(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "SurveyCon")))),
    surveyModel(QSharedPointer<SurveyTableModel>(new SurveyTableModel(this))),
    employeeModel(QSharedPointer<EmployeeTableModel>(new EmployeeTableModel(this))),
    dbLocation(""),
    currentEmpId(-1)
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
                              "name TEXT NOT NULL COLLATE NOCASE);");

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

    updateEmployeeTableModel();
    updateSurveyTableModel();

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
 * \brief Returns a pointer to the DB's employee model.
 * \return A QSqlQueryModel pointer of the model.
 * \note The returned pointer MUST NOT be deleted.
 */
QSqlQueryModel *SurveyDatabase::getEmployeeModel()
{
    return employeeModel.data();
}

/*!
 * \brief Retrieves the employee ID currently being used by the database.
 * \return An integer with the employee ID.
 */
int SurveyDatabase::getCurrentEmployeeId() const
{
    return currentEmpId;
}

/*!
 * \brief Assigns a new employee ID to use by the database.
 * \param id = The new employee ID
 * \note It will only assign the new ID if the new ID is -1 or larger. The value -1 means 'No ID' and any number that follows can be a valid SQLite ID.
 */
void SurveyDatabase::setCurrentEmployeeId(const int &id)
{
    if (id >= -1)
        currentEmpId = id;
}

/*!
 * \brief Adds a new employee to the database.
 * \param name = The name of the new employee
 * \return A boolean value that states whether the transaction was successful or not.
 * \note This will check if the name doesn't already exist and that it is not an empty string.
 * \note It uses the employeeExists() function to verify its existence.
 */
bool SurveyDatabase::addEmployee(const QString &name)
{
    if (name.length() > 0 && !employeeExist(name)) {
        openDb();

        QSqlQuery surveyQry(*surveyDb);

        surveyQry.prepare("INSERT INTO Employee (name) VALUES (?);");
        surveyQry.addBindValue(name);

        if (surveyQry.exec()) {
            closeDb();
            return true;
        } else {
            qDebug() << "(DB) Error adding new employee: " << surveyQry.lastError().text() << Qt::endl;
            closeDb();
        }
    }

    return false;
}

/*!
 * \brief Removes a given employee from the database.
 * \param empId = The employee's ID
 * \return A boolean value that states whether the transaction was successful or not.
 * \note This will also delete all surveys associated with this employee.
 */
bool SurveyDatabase::removeEmployee(const int &empId)
{
    openDb();

    QSqlQuery surveyQry(*surveyDb);

    // Delete all surveys belonging to this employee.
    surveyQry.prepare("DELETE FROM Survey "
                      "WHERE emp_id = :id;");
    surveyQry.bindValue(":id", empId);

    if (surveyQry.exec()) {

        // Delete this employee.
        surveyQry.prepare("DELETE FROM Employee "
                          "WHERE emp_id = :id;");

        surveyQry.bindValue(":id", empId);

        if (surveyQry.exec()) {
            closeDb();
            return true;
        } else
            qDebug() << "(DB) Error removing employee: " << surveyQry.lastError().text() << Qt::endl;

    } else
        qDebug() << "(DB) Error removing employee surveys: " << surveyQry.lastError().text() << Qt::endl;

    closeDb();
    return false;
}

/*!
 * \brief Adds a new survey to the database.
 * \param empId = The employee's ID
 * \param qOne = The answer to question 1
 * \param qTwo = The answer to question 2
 * \param qThree = The answer to question 3
 * \param temp = The employee's temperature
 * \return A boolean value that states whether the transaction was successful or not.
 * \note This will also check if a combination of this date and employee ID hasn't already been added. It will return false if it was.
 */
bool SurveyDatabase::addSurvey(const QDate &date, const int &empId, const bool &qOne, const bool &qTwo, const bool &qThree, const double &temp)
{
    openDb();

    QDateTime surveyDate(date, QTime(12,0));
    int surveyDateUnix(surveyDate.toSecsSinceEpoch());

    QSqlQuery surveyQry(*surveyDb);

    surveyQry.prepare("SELECT COUNT(*) FROM Survey WHERE survey_date = :date AND emp_id = :id;");
    surveyQry.bindValue(":date", surveyDateUnix);
    surveyQry.bindValue(":id", empId);

    if (surveyQry.exec()) {
        if (surveyQry.next()) {
            if (surveyQry.value(0).toInt() == 0) {

                surveyQry.prepare("INSERT INTO Survey (survey_date, emp_id, q_one, q_two, q_three, temperature) "
                                  "VALUES (:date, :id, :q1, :q2, :q3, :temp);");

                surveyQry.bindValue(":date", surveyDateUnix);
                surveyQry.bindValue(":id", empId);
                surveyQry.bindValue(":q1", qOne);
                surveyQry.bindValue(":q2", qTwo);
                surveyQry.bindValue(":q3", qThree);
                surveyQry.bindValue(":temp", temp);

                if (surveyQry.exec()) {
                    closeDb();
                    return true;
                } else
                    qDebug() << "(DB) Error adding survey: " << surveyQry.lastError().text() << Qt::endl;
            }
        }
    } else
        qDebug() << "(DB) Error verifying survey: " << surveyQry.lastError().text() << Qt::endl;

    closeDb();
    return false;
}

/*!
 * \brief Removes a survey from the database.
 * \param date = The survey date
 * \param empId = The employee's ID
 * \return A boolean value that states whether the transaction was successful or not.
 */
bool SurveyDatabase::removeSurvey(const QDate &date, const int &empId)
{
    openDb();

    QDateTime surveyDate(date, QTime(12, 0));
    int surveyDateUnix(surveyDate.toSecsSinceEpoch());

    QSqlQuery surveyQry(*surveyDb);

    surveyQry.prepare("DELETE FROM Survey "
                      "WHERE survey_date = :date AND emp_id = :id;");

    surveyQry.bindValue(":date", surveyDateUnix);
    surveyQry.bindValue(":id", empId);

    if (surveyQry.exec()) {
        closeDb();
        return true;
    } else
        qDebug() << "(DB) Error removing survey: " << surveyQry.lastError().text() << Qt::endl;

    closeDb();
    return false;
}

/*!
 * \brief Checks if a given employee name is already in the database.
 * \param name = The employee's name
 * \return A boolean value that is true if it does exist, and is false if it does not exist.
 * \note This is not case sensitive.
 */
bool SurveyDatabase::employeeExist(const QString &name)
{
    openDb();

    QSqlQuery surveyQry(*surveyDb);

    surveyQry.prepare("SELECT COUNT(*) FROM Employee WHERE name = :n;");
    surveyQry.bindValue(":n", name);

    if (surveyQry.exec()) {
        if (surveyQry.next()) {
            if (surveyQry.value(0).toInt() == 0) {
                closeDb();
                return false;
            }
        }
    } else
        qDebug() << "(DB) Error verifying employee name: " << surveyQry.lastError().text() << Qt::endl;

    closeDb();
    return true;
}

/*!
 * \brief Update the survey model with the current employee ID.
 *
 * Updates the survey model to display the current state of survey data from the current employee ID from currentEmpId.
 */
void SurveyDatabase::updateSurveyTableModel()
{
    openDb();

    surveyModel->setQuery("SELECT survey_date, q_one, q_two, q_three, temperature "
                          "FROM Survey "
                          "WHERE emp_id = " + QString::number(currentEmpId) + " "
                          "ORDER BY survey_date;", *surveyDb);

    surveyModel->setHeaderData(SurveyTableColumns::Date, Qt::Horizontal, tr("Survey Date"));
    surveyModel->setHeaderData(SurveyTableColumns::Question1, Qt::Horizontal, tr("Question 1"));
    surveyModel->setHeaderData(SurveyTableColumns::Question2, Qt::Horizontal, tr("Question 2"));
    surveyModel->setHeaderData(SurveyTableColumns::Question3, Qt::Horizontal, tr("Question 3"));
    surveyModel->setHeaderData(SurveyTableColumns::Temperature, Qt::Horizontal, tr("Temperature (°C)"));

    closeDb();
}

void SurveyDatabase::updateEmployeeTableModel()
{
    openDb();

    employeeModel->setQuery("SELECT emp_id, name FROM Employee ORDER BY name;", *surveyDb);

    employeeModel->setHeaderData(EmployeeTableColumns::ID, Qt::Horizontal, tr("ID"));
    employeeModel->setHeaderData(EmployeeTableColumns::Name, Qt::Horizontal, tr("Name"));

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
