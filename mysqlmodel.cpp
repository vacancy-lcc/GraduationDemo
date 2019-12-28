#include "mysqlmodel.h"
#include <QColor>
mysqlmodel::mysqlmodel(QObject *parent)
    :QSqlQueryModel(parent)
{

}

void mysqlmodel::fresh_info()
{
    setQuery("SELECT * FROM guns");
    setHeaderData(0, Qt::Horizontal, "卡号");
    setHeaderData(1, Qt::Horizontal, "时间");

}

Qt::ItemFlags mysqlmodel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f;
    return f;
}


// 自定义数据更新的模式
bool mysqlmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}



// 自定义数据呈现的模式
QVariant mysqlmodel::data(const QModelIndex &item, int role) const
{
    // 先将数据交给基类处理，得到一个从数据库文件获取的原生数据value
    QVariant value = QSqlQueryModel::data(item, role);

    // 凡是第四列中的颜色，一律为红色
    if(item.column() == 1 && role == Qt::TextColorRole)
    {
        return QColor(Qt::red);
    }

    // 凡是第一列中的对齐方式，一律为居中
    if(item.column() == 1 && role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    return value;
}
