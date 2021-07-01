#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../objects/surveydatabase.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void openSurveyDialog();
    void openEmployeeDialog();
    void addSurvey(const QDate& date,
                   const int& empId,
                   const bool& qOne,
                   const bool& qTwo,
                   const bool& qThree,
                   const double& temp);
    void removeSurvey(const QDate& date,
                      const int& empId);


private slots:
    void on_btnAddSurvey_clicked();

private:
    Ui::MainWindow *ui;         ///< The reference to the UI of the MainWindow.
    SurveyDatabase surveyDb;    ///< The database variable that stores the survey data.

    void setupSurveyTableContextMenu();
    int getCurrentEmployeeId();
};
#endif // MAINWINDOW_H
