#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

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
    void updateTableModel();
    QSqlQueryModel* getSurveyModel();

private:
    QSharedPointer<QSqlDatabase> surveyDb;      ///< The SQL Database variable where the data is stored.
    QSharedPointer<QSqlQueryModel> surveyModel; ///< The data model used to display data from the DB in a view.
    QString dbLocation;     ///< The full path to where the database file is stored.
    int currentEmpId;       ///< The current employee ID being focussed on.

    void openDb();
    void closeDb();
};

#endif // SURVEYDATABASE_H
