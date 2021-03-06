#include "employeedialog.h"
#include "ui_employeedialog.h"

#include "../objects/employeetablemodel.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>

/*!
 * \brief The constructor for the EmployeeDialog.
 * \param empModel = The model used to display the employees in a list
 * \param parent = The QWidget to which this dialog is bound to
 */
EmployeeDialog::EmployeeDialog(QSqlQueryModel *empModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeDialog),
    contextMenu(new QMenu(this))
{
    ui->setupUi(this);

    // Setup the listEmployees list.
    ui->listEmployees->setModel(empModel);
    ui->listEmployees->setModelColumn(EmployeeTableColumns::Name);

    // Setup the context menu for listEmployees.
    setupEmployeeListContextMenu();
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
 * \note This function only calls removeSelectedEmployee().
 */
void EmployeeDialog::on_btnDelete_clicked()
{
    if (ui->listEmployees->currentIndex().row() > -1)
        emit removeEmployee(getCurrentEmployeeId());
    else
        QMessageBox::information(this, "No entry selected", "Please select an entry from the list first.");
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
 * \brief Opens a context menu only if an item in the list is right clicked.
 * \param pos = The position of the mouse cursor when right clicked
 */
void EmployeeDialog::contextMenuRequested(const QPoint &pos)
{
    if (ui->listEmployees->indexAt(pos).row() > -1)
        contextMenu->popup(ui->listEmployees->viewport()->mapToGlobal(pos));
}

/*!
 * \brief Prompts a command to edit the currently selected employee entry.
 */
void EmployeeDialog::on_btnEdit_clicked()
{
    if (ui->listEmployees->currentIndex().row() > -1)
        emit editEmployee(getCurrentEmployeeId(), getCurrentEmployeeName());
    else
        QMessageBox::information(this, "No entry selected", "Please select an entry from the list first.");
}

/*!
 * \brief Create the context menu for the employee list.
 */
void EmployeeDialog::setupEmployeeListContextMenu()
{
    // Use listEmployees' actions as context menu items.
    ui->listEmployees->setContextMenuPolicy(Qt::CustomContextMenu);

    QAction* editAction(new QAction("Edit", this));
    connect(editAction, &QAction::triggered, [this]() {
        emit editEmployee(getCurrentEmployeeId(), getCurrentEmployeeName());
    });

    QAction* deleteAction(new QAction("Delete", this));
    connect(deleteAction, &QAction::triggered, [this]() {
        emit removeEmployee(getCurrentEmployeeId());
    });

    contextMenu->addAction(editAction);
    contextMenu->addAction(deleteAction);

    connect(ui->listEmployees, &QWidget::customContextMenuRequested, this, &EmployeeDialog::contextMenuRequested);
}

