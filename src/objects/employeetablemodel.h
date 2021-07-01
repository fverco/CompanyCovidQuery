#ifndef EMPLOYEETABLEMODEL_H
#define EMPLOYEETABLEMODEL_H

#include <QSqlQueryModel>

/*!
 * \brief Enum for the column headers found in the employee table.
 * \note The order in which they appear here is also their order in the table.
 */
enum EmployeeTableColumns {
    ID,     ///< 0
    Name    ///< 1
};

/*!
 * \brief The model for displaying the employees in the SurveyDatabase in a table.
 */
class EmployeeTableModel : public QSqlQueryModel
{
public:
    explicit EmployeeTableModel(QObject *parent = nullptr);
};

#endif // EMPLOYEETABLEMODEL_H
