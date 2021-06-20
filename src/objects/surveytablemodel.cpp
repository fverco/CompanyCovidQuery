#include "surveytablemodel.h"

#include <QDate>

/*!
 * \brief The constructor for the table model.
 * \param parent = The QObject to which this object is bound to
 */
SurveyTableModel::SurveyTableModel(QObject *parent) : QSqlQueryModel(parent)
{
}

/*!
 * \brief This function is used by the table view to retrieve and display individual items.
 * \param index = The current item index to be queried
 * \param role = The Qt::DisplayRole of the item index
 * \return A QVariant with the value of the item index to be displayed.
 */
QVariant SurveyTableModel::data(const QModelIndex &index, int role) const
{
    QVariant value(QSqlQueryModel::data(index, role));

    switch (index.column()) {
    case 0:
        return convertDateValue(value);
    case 1:
    case 2:
    case 3:
        return convertBoolValue(value);
    case 4:
        return convertDoubleValue(value);
    default:
        return QVariant(QString("Unknown type"));
    }

    return value;
}

/*!
 * \brief Converts an integer value from the table model to a simple "Yes" or "No" value.
 * \param val = The QVariant value of the item index
 * \return A QVariant value with the current item index value converted to "Yes" or "No".
 * \note Converts a 0 to "No" and 1 to "Yes".
 * \note If the value is not a 0 or 1, or not an integer, then it will simply return the value as is.
 */
QVariant SurveyTableModel::convertBoolValue(const QVariant &val) const
{
    QString valStr(val.toString());
    bool ok;
    int valInt(valStr.toInt(&ok));

    if (ok) {
        switch(valInt){
        case 0: return QVariant(QString("No"));
        case 1: return QVariant(QString("Yes"));
        default: return val;
        }
    }

    return val;
}

/*!
 * \brief Converts an integer value from the table model to a date string of the form "dd/MM/yyyy".
 * \param val = The QVariant value of the item index
 * \return A QVariant with the integer value converted to a date string.
 * \note If the value is not an ingeger value then it will return the value as is.
 */
QVariant SurveyTableModel::convertDateValue(const QVariant &val) const
{
    QString valStr(val.toString());
    bool ok;
    int valInt(valStr.toInt(&ok));

    if (ok)
        return QDateTime::fromSecsSinceEpoch(valInt).date().toString("dd/MM/yyyy");

    return val;
}

/*!
 * \brief Formats a double value to a precision of 1 with trailing zeros.
 * \param val = The QVariant value of the item index
 * \return A QVariant with the double value formatted.
 * \note If the value is not a double value then it will be returned as is.
 */
QVariant SurveyTableModel::convertDoubleValue(const QVariant &val) const
{
    QString valStr(val.toString());
    bool ok;
    double valDouble(valStr.toDouble(&ok));

    if (ok)
        return QString::number(valDouble, 'f', 1);

    return val;
}

