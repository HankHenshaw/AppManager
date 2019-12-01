#ifndef APPPROXYMODEL_H
#define APPPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QModelIndex>

class AppProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AppProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE int sourceRow(int proxyRow) const;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;
public slots:
    void slotSearchText(QVariant text);
};

#endif // APPPROXYMODEL_H
