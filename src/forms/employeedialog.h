#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class EmployeeDialog;
}

/*!
 * \brief The window where employee data is displayed, added, edited and removed.
 */
class EmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDialog(QSqlQueryModel *empModel, QWidget *parent = nullptr);
    ~EmployeeDialog();

private slots:
    void on_btnClose_clicked();
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();

signals:
    void addEmployee();
    void removeEmployee(const int &empId);

private:
    Ui::EmployeeDialog *ui;

    int getCurrentEmployeeId() const;
};

#endif // EMPLOYEEDIALOG_H
