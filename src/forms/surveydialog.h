#ifndef SURVEYDIALOG_H
#define SURVEYDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class SurveyDialog;
}

/*!
 * \brief The window where new survey data is entered, or existing survey data is changed.
 */
class SurveyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SurveyDialog(const int& emp, QWidget *parent = nullptr);
    ~SurveyDialog();

signals:
    void sendNewSurvey(const QDate& date,
                       const int& empId,
                       const bool& qOne,
                       const bool& qTwo,
                       const bool& qThree,
                       const double& temp);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void questionAnswered();

    void on_cbTodayDate_stateChanged(int arg1);

private:
    Ui::SurveyDialog *ui;   ///< The reference to the UI of the SurveyDialog.
    int empId; ///< The ID of the employee to which this survey belongs.
    QSharedPointer<QButtonGroup> rbgroupQOne;   ///< The button group for the question 1 radio buttons.
    QSharedPointer<QButtonGroup> rbgroupQTwo;   ///< The button group for the question 2 radio buttons.
    QSharedPointer<QButtonGroup> rbgroupQThree; ///< The button group for the question 3 radio buttons.
};

#endif // SURVEYDIALOG_H
