#ifndef SURVEYDIALOG_H
#define SURVEYDIALOG_H

#include "../objects/survey.h"

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
    explicit SurveyDialog(const int& emp, QWidget *parent = nullptr, const Survey &editSurvey = Survey());
    ~SurveyDialog();

signals:
    void sendNewSurvey(const Survey &newSurvey);
    void updateSurvey(const Survey &survey);

private slots:
    void questionAnswered();
    void on_cbTodayDate_stateChanged(int arg1);
    void answerChanged();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SurveyDialog *ui;   ///< The reference to the UI of the SurveyDialog.
    int empId; ///< The ID of the employee to which this survey belongs.
    QSharedPointer<QButtonGroup> rbgroupQOne;   ///< The button group for the question 1 radio buttons.
    QSharedPointer<QButtonGroup> rbgroupQTwo;   ///< The button group for the question 2 radio buttons.
    QSharedPointer<QButtonGroup> rbgroupQThree; ///< The button group for the question 3 radio buttons.
    bool editMode;  ///< Is the dialog in edit mode?

    void setupAddMode();
    void setupEditMode(const Survey &editSurvey);
};

#endif // SURVEYDIALOG_H
