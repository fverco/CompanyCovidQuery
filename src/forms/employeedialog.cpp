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


void EmployeeDialog::on_btnDelete_clicked()
{
    emit removeEmployee(getCurrentEmployeeId());
}

int EmployeeDialog::getCurrentEmployeeId() const
{
    int row(ui->listEmployees->currentIndex().row());
    QModelIndex index = ui->listEmployees->model()->index(row, EmployeeTableColumns::ID);
    QVariant id(ui->listEmployees->model()->data(index));

    return id.toInt();
}

