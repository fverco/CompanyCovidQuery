#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

#include "surveytablemodel.h"
#include "employeetablemodel.h"
#include "survey.h"

#include <QSharedPointer>
#include <QGuiApplication>

class QSqlDatabase;
class QSqlQueryModel;

/*!
 * \brief The database class for storing survey data.
 */
class SurveyDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SurveyDatabase(QObject *parent = nullptr);
    bool createDatabase(const QString &dir = QGuiApplication::applicationDirPath() + "/survey.data");
    void updateSurveyTableModel();
    void updateEmployeeTableModel();

    QSqlQueryModel *getSurveyModel();
    QSqlQueryModel *getEmployeeModel();
    int getCurrentEmployeeId() const;

    void setCurrentEmployeeId(const int &id);

    bool addEmployee(const QString &name);
    bool removeEmployee(const int &empId);
    bool editEmployee(const int &empId, const QString &newName);
    bool editEmployee(const QString &currentName, const QString &newName);

    bool addSurvey(const Survey &newSurvey);
    bool removeSurvey(const QDate &date,
                      const int &empId);
    bool editSurvey(const Survey &editSurvey);

    bool employeeExist(const QString &name);

private:
    QSharedPointer<QSqlDatabase> surveyDb;      ///< The SQL Database variable where the data is stored.
    QSharedPointer<SurveyTableModel> surveyModel; ///< The data model used to display survey data from the DB in a view.
    QSharedPointer<EmployeeTableModel> employeeModel; ///< The data model used to display employee data from the DB in a view.
    QString dbLocation;     ///< The full path to where the database file is stored.
    int currentEmpId;       ///< The current employee ID being focussed on.

    void openDb();
    void closeDb();
};

#endif // SURVEYDATABASE_H
