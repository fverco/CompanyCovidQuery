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
    bool createTableModel();

private:
    QSharedPointer<QSqlDatabase> surveyDb;
    QSharedPointer<QSqlTableModel> surveyModel;
    QString dbLocation;

    void openDb();
    void closeDb();
};

#endif // SURVEYDATABASE_H
