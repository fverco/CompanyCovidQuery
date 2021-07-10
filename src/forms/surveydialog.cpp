#include "surveydialog.h"
#include "ui_surveydialog.h"

#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>

/*!
 * \brief The constructor for the SurveyDialog.
 * \param parent = The QWidget to which this dialog is bound to.
 */
SurveyDialog::SurveyDialog(const int& emp, QWidget *parent, const Survey &editSurvey) :
    QDialog(parent),
    ui(new Ui::SurveyDialog),
    empId(emp),
    rbgroupQOne(new QButtonGroup(this)),
    rbgroupQTwo(new QButtonGroup(this)),
    rbgroupQThree(new QButtonGroup(this)),
    editMode(false)
{
    ui->setupUi(this);

    // Add all the "Yes" and "No" radio buttons to their respective button groups.
    rbgroupQOne->addButton(ui->rbQOneYes);
    rbgroupQOne->addButton(ui->rbQOneNo);
    rbgroupQTwo->addButton(ui->rbQTwoYes);
    rbgroupQTwo->addButton(ui->rbQTwoNo);
    rbgroupQThree->addButton(ui->rbQThreeYes);
    rbgroupQThree->addButton(ui->rbQThreeNo);

    // If a survey is about to be edited instead...
    if (editSurvey.isValid())
        setupEditMode(editSurvey);
    else
        setupAddMode();
}

/*!
 * \brief The destructor for the SurveyDialog.
 */
SurveyDialog::~SurveyDialog()
{
    delete ui;
}

/*!
 * \brief Whenever a radio button is toggled (question is answered) it will determine if all questions is answered and enable the Add button if true.
 * \note This function is used only if the SurveyDialog is adding a new survey.
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

/*!
 * \brief This function will enable the "Add" or "Apply Changes" button if any of the answers in the survey is changed by the user.
 * \note This function is used only if the SurveyDialog is updating an existing survey.
 */
void SurveyDialog::answerChanged()
{
    if (!ui->buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
        ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(false);
}

/*!
 * \brief Setup all UI elements for adding a new survey.
 */
void SurveyDialog::setupAddMode()
{
    // Connect the "toggled" signals of all button groups with the questionAnswered() function.
    connect(rbgroupQOne.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);
    connect(rbgroupQTwo.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);
    connect(rbgroupQThree.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::questionAnswered);

    // Set the default date to today and give the date edit a calendar popup.
    ui->cbTodayDate->setCheckState(Qt::CheckState::Checked);
    ui->deSurveyDate->setCalendarPopup(true);

    // Set the "OK" button text to "Add" and disable it.
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Add");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
}

/*!
 * \brief Setup all UI elements for editing an existing survey.
 * \param editSurvey = The survey to be edited
 * \note This does not check if the survey is valid.
 */
void SurveyDialog::setupEditMode(const Survey &editSurvey)
{
    // Set the default date to today and give the date edit a calendar popup.
    ui->cbTodayDate->setCheckState(Qt::CheckState::Unchecked);
    ui->cbTodayDate->setDisabled(true);
    ui->deSurveyDate->setDate(editSurvey.getSurveyDate());
    ui->deSurveyDate->setDisabled(true);

    // Set all answers to "No" by default.
    ui->rbQOneNo->setChecked(true);
    ui->rbQTwoNo->setChecked(true);
    ui->rbQThreeNo->setChecked(true);

    // Set the "OK" button text to "Add" and disable it.
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Apply Changes");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);

    // Assign the answers of the survey to the UI.
    // NOTE: All answers MUST be set to "No" beforehand for this to work.
    ui->rbQOneYes->setChecked(editSurvey.getQuestionOne());
    ui->rbQTwoYes->setChecked(editSurvey.getQuestionTwo());
    ui->rbQThreeYes->setChecked(editSurvey.getQuestionThree());
    ui->dspinTemp->setValue(editSurvey.getTemperature());

    // Connect the "toggled" and "valueChanged" signals of questions with the answerChanged() function.
    connect(rbgroupQOne.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::answerChanged);
    connect(rbgroupQTwo.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::answerChanged);
    connect(rbgroupQThree.get(), &QButtonGroup::buttonToggled, this, &SurveyDialog::answerChanged);
    connect(ui->dspinTemp, &QDoubleSpinBox::valueChanged, this, &SurveyDialog::answerChanged);

    this->setWindowTitle(tr("Editing survey"));

    editMode = true;
}

/*!
 * \brief If the "Add" or "Apply Changes" is clicked, the survey will be added or updated.
 */
void SurveyDialog::on_buttonBox_accepted()
{
    Survey survey(ui->deSurveyDate->date(),
                     empId,
                     rbgroupQOne->checkedButton()->text() == "Yes",
                     rbgroupQTwo->checkedButton()->text() == "Yes",
                     rbgroupQThree->checkedButton()->text() == "Yes",
                     ui->dspinTemp->value());

    if (editMode)
        emit updateSurvey(survey);
    else
        emit sendNewSurvey(survey);
}

/*!
 * \brief If the "Cancel" button is clicked, the window will be closed.
 */
void SurveyDialog::on_buttonBox_rejected()
{
    this->close();
}

