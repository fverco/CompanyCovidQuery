#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../objects/surveydatabase.h"
#include "../objects/survey.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QMenu;

/*!
 * \brief The main window to be used in the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateEmployeeComboBox();
    void updateSurveyTableModel();
    void addEmployee();
    void removeEmployee(const int &empId);
    void editEmployee(const int &empId, const QString &currentName);
    void openSurveyDialog(const Survey &newSurvey = Survey());
    void openEmployeeDialog();
    void addSurvey(const Survey &newSurvey);
    void removeSurvey(const QDate &date,
                      const int &empId);
    void editSurvey(const Survey &survey);


private slots:
    void on_btnAddSurvey_clicked();

private:
    Ui::MainWindow *ui;         ///< The reference to the UI of the MainWindow.
    SurveyDatabase surveyDb;    ///< The database variable that stores the survey data.
    QMenu *contextMenu;

    void setupSurveyTableContextMenu();
    int getCurrentEmployeeId() const;
    Survey getCurrentSurvey();
    QDate getCurrentSurveyDate();
    void contextMenuRequested(const QPoint &pos);
};
#endif // MAINWINDOW_H
