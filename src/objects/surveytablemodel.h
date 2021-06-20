#ifndef SURVEYTABLEMODEL_H
#define SURVEYTABLEMODEL_H

#include <QSqlQueryModel>

/*!
 * \brief The model for displaying the SurveyDatabase items in a table.
 */
class SurveyTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit SurveyTableModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

private:
    QVariant convertBoolValue(const QVariant &val) const;
    QVariant convertDateValue(const QVariant &val) const;
    QVariant convertDoubleValue(const QVariant &val) const;
};

#endif // SURVEYTABLEMODEL_H
