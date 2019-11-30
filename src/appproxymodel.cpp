#include "appproxymodel.h"

AppProxyModel::AppProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool AppProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool result = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    QModelIndex currentIndex = sourceModel()->index(source_row, 0, source_parent);
    if(sourceModel()->hasChildren(currentIndex))
        for(int i = 0; i < sourceModel()->rowCount(currentIndex) && !result; ++i)
            result = result || filterAcceptsRow(i, currentIndex);
    return result;
}

bool AppProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

    return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
}

void AppProxyModel::slotSearchText(QVariant text)
{
    QRegExp filterExpr(text.toString(), Qt::CaseInsensitive, QRegExp::RegExp);
    setFilterRegExp(filterExpr);
}