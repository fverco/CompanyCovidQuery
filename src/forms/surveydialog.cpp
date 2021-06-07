#include "surveydialog.h"
#include "ui_surveydialog.h"

SurveyDialog::SurveyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurveyDialog)
{
    ui->setupUi(this);
}

SurveyDialog::~SurveyDialog()
{
    delete ui;
}
