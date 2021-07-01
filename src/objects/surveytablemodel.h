#ifndef SURVEYTABLEMODEL_H
#define SURVEYTABLEMODEL_H

#include <QSqlQueryModel>

/*!
 * \brief Enum for the column headers found in the survey table.
 * \note The order in which they appear here is also their order in the table.
 */
enum SurveyTableColumns {
    Date,           ///< 0
    Question1,      ///< 1
    Question2,      ///< 2
    Question3,      ///< 3
    Temperature     ///< 4
};

/*!
 * \brief The model for displaying the SurveyDatabase items in a table.
 */
class SurveyTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SurveyTableModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

private:
    QVariant convertBoolValue(const QVariant &val) const;
    QVariant convertDateValue(const QVariant &val) const;
    QVariant convertDoubleValue(const QVariant &val) const;
};

#endif // SURVEYTABLEMODEL_H
