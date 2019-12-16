#include "appmodel.h"
#include <QDebug>

AppModel::AppModel(ImageProvider *ip, QObject *parent)
    : QAbstractListModel(parent),
      m_imgProvider(ip)
{
    m_native.getPackageName();
    m_native.getAppLabel();
    m_native.getAppIcon();

    m_nameList = m_native.getListOfPackLabel();
    m_imgProvider->fillMap(m_native.getListOfPackIcon());

    connect(this, SIGNAL(sigRunApp(QVariant)), &m_native, SLOT(slotRunApp(QVariant)));
    connect(this, SIGNAL(signalAppInfo(QVariant)), &m_native, SLOT(slotAppInfo(QVariant)));
}

int AppModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << "rowCount =" << m_nameList.size();
    Q_UNUSED(parent)
    return m_nameList.size();
}

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "Get Data";

    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
        return m_nameList.at(index.row());

    return QVariant();
}

QVariant AppModel::getCategory(QVariant index)
{
    return m_native.getCategory(index);
}

QVariant AppModel::getVersion(QVariant index)
{
    return m_native.getVersion(index);
}

QVariant AppModel::getMinSdk(QVariant index)
{
    return m_native.getMinSdk(index);
}

QVariant AppModel::getTargetSdk(QVariant index)
{
    return m_native.getTargetSdk(index);
}

QVariant AppModel::getPackagePath(QVariant index)
{
    return m_native.getPackagePath(index);
}

QVariant AppModel::getPackageName(QVariant index)
{
    return m_native.getPackageName(index);
}

QVariant AppModel::getPackageSize(QVariant index)
{
    return m_native.getPackageSize(index);
}

QVariant AppModel::getCacheSize(QVariant index)
{
    return m_native.getCacheSize(index);
}

QVariant AppModel::getRequestedPermissions(QVariant index)
{
    return m_native.getRequestedPermissions(index);
}

QVariant AppModel::getPermissionsNumber()
{
    return m_native.getPermissionsNumber();
}
