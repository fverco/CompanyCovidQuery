#include "employeedialog.h"
#include "ui_employeedialog.h"

#include "../objects/employeetablemodel.h"

/*!
 * \brief The constructor for the EmployeeDialog.
 * \param empModel = The model used to display the employees in a list
 * \param parent = The QWidget to which this dialog is bound to
 */
EmployeeDialog::EmployeeDialog(QSqlQueryModel *empModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeDialog)
{
    ui->setupUi(this);

    ui->listEmployees->setModel(empModel);
    ui->listEmployees->setModelColumn(EmployeeTableColumns::Name);
}

/*!
 * \brief The destructor for the EmployeeDialog.
 */
EmployeeDialog::~EmployeeDialog()
{
    delete ui;
}

/*!
 * \brief Closes the dialog.
 */
void EmployeeDialog::on_btnClose_clicked()
{
    this->close();
}

/*!
 * \brief Prompts an operation to add a new employee.
 */
void EmployeeDialog::on_btnAdd_clicked()
{
    emit addEmployee();
}

/*!
 * \brief Deletes the selected employee.
 */
void EmployeeDialog::on_btnDelete_clicked()
{
    emit removeEmployee(getCurrentEmployeeId());
}

/*!
 * \brief Retrieves the ID of the employee currently selected.
 * \return An integer value with the employee's ID.
 */
int EmployeeDialog::getCurrentEmployeeId() const
{
    int row(ui->listEmployees->currentIndex().row());
    QModelIndex index(ui->listEmployees->model()->index(row, EmployeeTableColumns::ID));
    QVariant id(ui->listEmployees->model()->data(index));

    return id.toInt();
}

/*!
 * \brief Retrieves the name of the employee currently selected.
 * \return A QString value with the employee's name.
 */
QString EmployeeDialog::getCurrentEmployeeName() const
{
    int row(ui->listEmployees->currentIndex().row());
    QModelIndex index(ui->listEmployees->model()->index(row, EmployeeTableColumns::Name));
    QVariant name(ui->listEmployees->model()->data(index));

    return name.toString();
}

/*!
 * \brief Prompts a command to edit the currently selected employee entry.
 */
void EmployeeDialog::on_btnEdit_clicked()
{
    emit editEmployee(getCurrentEmployeeId(), getCurrentEmployeeName());
}

