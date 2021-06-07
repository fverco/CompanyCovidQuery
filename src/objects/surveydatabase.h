#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

#include <QSharedPointer>

class QSqlDatabase;
class QSqlTableModel;

class SurveyDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SurveyDatabase(QObject *parent = nullptr);

private:
    QSharedPointer<QSqlDatabase> surveyDb;
    QSharedPointer<QSqlTableModel> surveyModel;
    QString dbLocation;

    void createDatabase();
    void createTableModel();
    void openDb();
    void closeDb();
};

#endif // SURVEYDATABASE_H
