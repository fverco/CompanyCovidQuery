#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

#include <QSharedPointer>
#include <QGuiApplication>

class QSqlDatabase;
class QSqlQueryModel;

class SurveyDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SurveyDatabase(QObject *parent = nullptr);
    bool createDatabase(const QString &dir = QGuiApplication::applicationDirPath() + "/survey.data");
    void updateTableModel();
    QSqlQueryModel* getSurveyModel();

private:
    QSharedPointer<QSqlDatabase> surveyDb;
    QSharedPointer<QSqlQueryModel> surveyModel;
    QString dbLocation;
    int currentEmpId;

    void openDb();
    void closeDb();
};

#endif // SURVEYDATABASE_H
