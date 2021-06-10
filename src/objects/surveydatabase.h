#ifndef SURVEYDATABASE_H
#define SURVEYDATABASE_H

#include <QSharedPointer>
#include <QGuiApplication>

class QSqlDatabase;
class QSqlTableModel;

class SurveyDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SurveyDatabase(QObject *parent = nullptr);
    bool createDatabase(const QString &dir = QGuiApplication::applicationDirPath() + "/survey.data");
    QSqlTableModel* getSurveyModel();

private:
    QSharedPointer<QSqlDatabase> surveyDb;
    QSharedPointer<QSqlTableModel> surveyModel;
    QString dbLocation;
    int currentEmpId;

    void openDb();
    void closeDb();
    void createTableModel();
    void updateTableModel();
};

#endif // SURVEYDATABASE_H
