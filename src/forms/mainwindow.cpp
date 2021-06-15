#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlTableModel>
#include <QMessageBox>

/*!
 * \brief The constructor for the MainWindow.
 * \param parent = The QWidget to which this window is bound to.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      surveyDb()
{
    ui->setupUi(this);

    connect(ui->comboEmployee, &QComboBox::currentIndexChanged, this, &MainWindow::updateSurveyTableModel);
    connect(ui->actionNewEmployee, &QAction::triggered, this, &MainWindow::addNewEmployee);

    if (!surveyDb.createDatabase())
        QApplication::quit();

    updateEmployeeComboBox();

    ui->tableSurveys->setModel(surveyDb.getSurveyModel());
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
void MainWindow::addNewEmployee()
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

