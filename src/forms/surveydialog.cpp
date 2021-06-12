#include "surveydialog.h"
#include "ui_surveydialog.h"

/*!
 * \brief The constructor for the SurveyDialog.
 * \param parent = The QWidget to which this dialog is bound to.
 */
SurveyDialog::SurveyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurveyDialog)
{
    ui->setupUi(this);
}

/*!
 * \brief The destructor for the SurveyDialog.
 */
SurveyDialog::~SurveyDialog()
{
    delete ui;
}
