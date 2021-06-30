#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "surveydialog.h"

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

    // Connect the new employee action button.
    connect(ui->actionNewEmployee, &QAction::triggered, this, &MainWindow::addEmployee);

    // Setup the database.
    if (!surveyDb.createDatabase())
        QApplication::quit();

    updateEmployeeComboBox();

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
 * \note This changes the currentEmpId of the surveyDb.
 * \note After updating the Employee ComboBox it will implicitly update the survey table. This happens ONLY if you have connected the combobox's signal with this class' updateSurveyTableModel slot.
 */
void MainWindow::updateEmployeeComboBox()
{
    ui->comboEmployee->clear();

    QMap<int, QString> empList(surveyDb.getEmployees());
    QMapIterator<int, QString> i(empList);

    if (empList.size() > 0) {
        while(i.hasNext()) {
            i.next();
            ui->comboEmployee->addItem(i.value(), i.key());
        }
    } else
        ui->comboEmployee->addItem("-", -1);
}

/*!
 * \brief Updates the survey table model with the current selected employee in the Employee ComboBox.
 */
void MainWindow::updateSurveyTableModel()
{
    surveyDb.setCurrentEmployeeId(ui->comboEmployee->currentData().toInt());
    surveyDb.updateTableModel();
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
 * \brief Opens the survey dialog
 * This will also assign the current selected employee's ID as the ID to which the new survey will belong to.
 */
void MainWindow::openSurveyDialog()
{
    SurveyDialog *surveyDialog(new SurveyDialog(ui->comboEmployee->currentData().toInt() , this));

    connect(surveyDialog, &SurveyDialog::sendNewSurvey, this, &MainWindow::addSurvey);

    surveyDialog->setAttribute(Qt::WA_DeleteOnClose);
    surveyDialog->open();
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
            QDate surveyDate(QDate::fromString(surveyRecord.value(TableColumns::Date).toString(), "dd/MM/yyyy"));

            // Get the current employee's ID.
            int surveyEmpId(ui->comboEmployee->currentData().toInt());

            removeSurvey(surveyDate, surveyEmpId);
        }
    });


    ui->tableSurveys->addAction(deleteAction);
}

