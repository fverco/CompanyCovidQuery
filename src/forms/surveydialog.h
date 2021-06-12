#ifndef SURVEYDIALOG_H
#define SURVEYDIALOG_H

#include <QDialog>

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
    explicit SurveyDialog(QWidget *parent = nullptr);
    ~SurveyDialog();

private:
    Ui::SurveyDialog *ui;   ///< The reference to the UI of the SurveyDialog.
};

#endif // SURVEYDIALOG_H
