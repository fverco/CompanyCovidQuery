#include "surveydialog.h"
#include "ui_surveydialog.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>
#include <QDate>

/*!
 * \brief The constructor for the SurveyDialog.
 * \param parent = The QWidget to which this dialog is bound to.
 */
SurveyDialog::SurveyDialog(const int& emp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurveyDialog),
    empId(emp),
    rbgroupQOne(new QButtonGroup(this)),
    rbgroupQTwo(new QButtonGroup(this)),
    rbgroupQThree(new QButtonGroup(this))
{
    ui->setupUi(this);

    // Set the "OK" button text to "Add" and disable it.
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Add");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    // Connect the "toggled" signals of all button groups with the questionAnswered() function.
    connect(rbgroupQOne.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);
    connect(rbgroupQTwo.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);
    connect(rbgroupQThree.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);

    // Add all the "Yes" and "No" radio buttons to their respective button groups.
    rbgroupQOne->addButton(ui->rbQOneYes);
    rbgroupQOne->addButton(ui->rbQOneNo);
    rbgroupQTwo->addButton(ui->rbQTwoYes);
    rbgroupQTwo->addButton(ui->rbQTwoNo);
    rbgroupQThree->addButton(ui->rbQThreeYes);
    rbgroupQThree->addButton(ui->rbQThreeNo);

    // Set the default date to today and give the date edit a calendar popup.
    ui->cbTodayDate->setCheckState(Qt::CheckState::Checked);
    ui->deSurveyDate->setCalendarPopup(true);
}

/*!
 * \brief The destructor for the SurveyDialog.
 */
SurveyDialog::~SurveyDialog()
{
    delete ui;
}

/*!
 * \brief Determines whether Add or Cancel is clicked and performs the appropriate action.
 * \param button = The button that was clicked
 */
void SurveyDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Add") {

        QDate survDate(ui->deSurveyDate->date());
        bool qOne(rbgroupQOne->checkedButton()->text() == "Yes"),
             qTwo(rbgroupQTwo->checkedButton()->text() == "Yes"),
             qThree(rbgroupQThree->checkedButton()->text() == "Yes");
        double temp(ui->dspinTemp->value());

        emit sendNewSurvey(survDate, empId, qOne, qTwo, qThree, temp);

    } else if (button->text() == "Cancel")
        this->close();
}

/*!
 * \brief Whenever a radio button is toggled (question is answered) it will determine if all questions is answered and enable the Add button if true.
 */
void SurveyDialog::questionAnswered()
{
    if (rbgroupQOne->checkedButton() != nullptr &&
            rbgroupQTwo->checkedButton() != nullptr &&
            rbgroupQThree->checkedButton() != nullptr) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(false);
    }
}

/*!
 * \brief Executes whenever the "Today" checkbox is ticked.
 * \param arg1 = The state of the checkbox after being ticked
 * \note This will set the date to today, and disable the date edit if checked.
 */
void SurveyDialog::on_cbTodayDate_stateChanged(int arg1)
{
    if (arg1 == Qt::CheckState::Checked) {
        ui->deSurveyDate->setDate(QDate::currentDate());
        ui->deSurveyDate->setEnabled(false);
    } else
        ui->deSurveyDate->setEnabled(true);
}
