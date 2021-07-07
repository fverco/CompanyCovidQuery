#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surveydialog.h"
#include "employeedialog.h"

#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlRecord>

/*!
 * \brief The constructor for the MainWindow.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      surveyDb()
{
    // Initialize the UI.
    ui->setupUi(this);

    // Update the survey table when a new employee is selected.
    connect(ui->comboEmployee, &QComboBox::currentIndexChanged, this, &MainWindow::updateSurveyTableModel);

    // Connect the new employee and employee list action buttons.
    connect(ui->actionNewEmployee, &QAction::triggered, this, &MainWindow::addEmployee);
    connect(ui->actionEmployeeList, &QAction::triggered, this, &MainWindow::openEmployeeDialog);

    // Setup the database.
    if (!surveyDb.createDatabase())
        QApplication::quit();

    // Set the model for the employee combobox.
    ui->comboEmployee->setModel(surveyDb.getEmployeeModel());
    ui->comboEmployee->setModelColumn(EmployeeTableColumns::Name);

    // Set the model for the survey table.
    ui->tableSurveys->setModel(surveyDb.getSurveyModel());

    setupSurveyTableContextMenu();
}

/*!
 * \brief The destructor for the MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Retrieves a list of all employees and assign it to the Employee ComboBox in the UI.
 * \note After updating the Employee ComboBox it will implicitly update the survey table. This happens ONLY if you have connected the combobox's signal with this class' updateSurveyTableModel slot.
 */
void MainWindow::updateEmployeeComboBox()
{
    surveyDb.updateEmployeeTableModel();
}

/*!
 * \brief Updates the survey table model with the current selected employee in the Employee ComboBox.
 */
void MainWindow::updateSurveyTableModel()
{
    surveyDb.setCurrentEmployeeId(getCurrentEmployeeId());
    surveyDb.updateSurveyTableModel();
}

/*!
 * \brief Asks user for an employee name and adds it to the database.
 */
void MainWindow::addEmployee()
{
    bool ok;
    QString empName = QInputDialog::getText(this, tr("New Employee"),
                                            tr("Employee's name:"), QLineEdit::Normal,
                                            "", &ok);

    if (ok && !empName.isEmpty()) {
        if (surveyDb.addEmployee(empName)) {
            updateEmployeeComboBox();
            QMessageBox::information(this, tr("Success"), tr("The new employee has been successfully added."));
        } else
            QMessageBox::critical(this, tr("Error"), tr("An unexpected error has ocurred when adding the new employee."));
    }
}

/*!
 * \brief Deletes the given employee from the database.
 * \param empId = The employee's ID
 * \note This does generate a confirmation message before deletion.
 */
void MainWindow::removeEmployee(const int &empId)
{

    QMessageBox::StandardButton buttonPressed(QMessageBox::question(this,
                                                "Delete Employee",
                                                "Are you sure you wish to delete this employee?\nCaution: This will delete all their surveys as well."));

    if (buttonPressed == QMessageBox::StandardButton::Yes) {
        if (surveyDb.removeEmployee(empId)) {
            updateEmployeeComboBox();
            updateSurveyTableModel();
        } else
            QMessageBox::critical(this, tr("Error"), tr("An unexpected error has ocurred while removing the employee."));
    }
}

/*!
 * \brief Displays an existing employee's name and allows the user to edit it.
 * \param empId = The employee's ID
 * \param currentName = The name of the employee before the change
 */
void MainWindow::editEmployee(const int &empId, const QString &currentName)
{
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Edit") + " " + currentName,
                                            tr("Employee's name:"), QLineEdit::Normal,
                                            currentName, &ok);

    if (ok && !newName.isEmpty()) {
        if (surveyDb.editEmployee(empId, newName)) {
            updateEmployeeComboBox();
            QMessageBox::information(this, tr("Success"), currentName + " " + tr("has been successfully renamed to") + " " + newName + ".");
        } else
            QMessageBox::critical(this, tr("Error"), tr("An unexpected error ocurred while changing") + " " + currentName + tr("'s name."));
    }
}

/*!
 * \brief Opens the survey dialog
 * This will also assign the current selected employee's ID as the ID to which the new survey will belong to.
 */
void MainWindow::openSurveyDialog()
{
    SurveyDialog *surveyDialog(new SurveyDialog(getCurrentEmployeeId(), this));

    connect(surveyDialog, &SurveyDialog::sendNewSurvey, this, &MainWindow::addSurvey);

    surveyDialog->setAttribute(Qt::WA_DeleteOnClose);
    surveyDialog->open();
}

/*!
 * \brief Opens the employee dialog
 */
void MainWindow::openEmployeeDialog()
{
    EmployeeDialog *employeeDialog(new EmployeeDialog(surveyDb.getEmployeeModel()));

    connect(employeeDialog, &EmployeeDialog::addEmployee, this, &MainWindow::addEmployee);
    connect(employeeDialog, &EmployeeDialog::removeEmployee, this, &MainWindow::removeEmployee);
    connect(employeeDialog, &EmployeeDialog::editEmployee, this, &MainWindow::editEmployee);

    employeeDialog->setAttribute(Qt::WA_DeleteOnClose);
    employeeDialog->open();
}

/*!
 * \brief Adds the new data as a new survey in the database.
 * \param empId = The ID of the employee
 * \param qOne = The answer to question 1
 * \param qTwo = The answer to question 2
 * \param qThree = The answer to question 3
 * \param temp = The employee's temperature
 * \note This will automatically update the survey table if successful.
 */
void MainWindow::addSurvey(const QDate &survDate, const int &empId, const bool &qOne, const bool &qTwo, const bool &qThree, const double &temp)
{
    if (surveyDb.addSurvey(survDate, empId, qOne, qTwo, qThree, temp)) {
        updateSurveyTableModel();
        QMessageBox::information(this, tr("Success"), tr("The new survey has been successfully added."));
    } else
        QMessageBox::critical(this, tr("Error"), tr("An unexpected error has ocurred when adding the new survey."));
}

/*!
 * \brief Removes the survey that contains the given date and employee ID.
 * \param date = The survey date
 * \param empId = The employee's ID
 * \note This does not generate a confirmation message. It only generates an error message if the action failed.
 */
void MainWindow::removeSurvey(const QDate &date, const int &empId)
{
    if (surveyDb.removeSurvey(date, empId))
        updateSurveyTableModel();
    else
        QMessageBox::critical(this, tr("Error"), tr("An unexpected error has ocurred while removing the survey."));
}

/*!
 * \brief The function executed when clicking the Add Survey button.
 * This function will call the openSurveyDialog() function.
 */
void MainWindow::on_btnAddSurvey_clicked()
{
    openSurveyDialog();
}

/*!
 * \brief Create the context menu for the survey table.
 */
void MainWindow::setupSurveyTableContextMenu()
{
    // Use tableSurveys' actions as context menu items.
    ui->tableSurveys->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction* deleteAction(new QAction("Delete", this));
    connect(deleteAction, &QAction::triggered, [this]() {
        if (QMessageBox::question(this, "Delete survey?",
                                  "Are you sure you wish to delete this survey?") == QMessageBox::Yes)
        {
            QModelIndexList indexList(ui->tableSurveys->selectionModel()->selectedRows());

            // Retrieve the first row in the list.
            // Only one selected row should be allowed.
            QModelIndex index(indexList[0]);

            // Retrieve the record of the row in the survey model.
            QSqlRecord surveyRecord(surveyDb.getSurveyModel()->record(index.row()));

            // Convert the formatted string date to a QDate value.
            QDate surveyDate(QDate::fromString(surveyRecord.value(SurveyTableColumns::Date).toString(), "dd/MM/yyyy"));

            removeSurvey(surveyDate, getCurrentEmployeeId());
        }
    });


    ui->tableSurveys->addAction(deleteAction);
}

/*!
 * \brief Retrieves the ID of the employee currently selected in the employee combobox.
 * \return An integer with the employee's ID
 */
int MainWindow::getCurrentEmployeeId() const
{
    int row(ui->comboEmployee->currentIndex());
    QModelIndex index = ui->comboEmployee->model()->index(row, EmployeeTableColumns::ID);
    QVariant id(ui->comboEmployee->model()->data(index));

    return id.toInt();
}

