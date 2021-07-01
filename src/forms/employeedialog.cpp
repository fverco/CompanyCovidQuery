#include "employeedialog.h"
#include "ui_employeedialog.h"

#include "../objects/employeetablemodel.h"

EmployeeDialog::EmployeeDialog(QSqlQueryModel *empModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);

    ui->listEmployees->setModel(empModel);
    ui->listEmployees->setModelColumn(EmployeeTableColumns::Name);
}

EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

void EmployeeDialog::on_btnClose_clicked()
{
    this->close();
}

void EmployeeDialog::on_btnAdd_clicked()
{
    emit addEmployee();
}

