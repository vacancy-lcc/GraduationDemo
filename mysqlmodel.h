#ifndef MYSQLMODEL_H
#define MYSQLMODEL_H

#include <QObject>
#include <QSqlQueryModel>

class mysqlmodel : public QSqlQueryModel
{
    Q_OBJECT
public:
    mysqlmodel(QObject *parent);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &item, int role) const override;
    void fresh_info();
};

#endif // MYSQLMODEL_H
