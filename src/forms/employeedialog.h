#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class EmployeeDialog;
}

class QMenu;

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
    void on_btnEdit_clicked();

signals:
    void addEmployee();
    void removeEmployee(const int &empId);
    void editEmployee(const int &empId, const QString &currentName);

private:
    Ui::EmployeeDialog *ui;
    QMenu *contextMenu;

    void setupEmployeeListContextMenu();
    int getCurrentEmployeeId() const;
    QString getCurrentEmployeeName() const;
    void contextMenuRequested(const QPoint &pos);
};

#endif // EMPLOYEEDIALOG_H
