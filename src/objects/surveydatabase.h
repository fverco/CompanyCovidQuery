#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

#include "surveytablemodel.h"
#include "employeetablemodel.h"

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
    bool addSurvey(const QDate &date,
                   const int &empId,
                   const bool &qOne,
                   const bool &qTwo,
                   const bool &qThree,
                   const double &temp);
    bool removeSurvey(const QDate &date,
                      const int &empId);

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
