#include "employeetablemodel.h"

/*!
 * \brief The constructor for the table model.
 * \param parent = The QObject to which this object is bound to
 */
EmployeeTableModel::EmployeeTableModel(QObject *parent) : QSqlQueryModel(parent)
{
}
