#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../objects/surveydatabase.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SurveyDatabase surveyDb;
};
#endif // MAINWINDOW_H
