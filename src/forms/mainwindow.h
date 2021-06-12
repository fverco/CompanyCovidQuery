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

private:
    Ui::MainWindow *ui;         ///< The reference to the UI of the MainWindow.
    SurveyDatabase surveyDb;    ///< The database variable that stores the survey data.
};
#endif // MAINWINDOW_H
