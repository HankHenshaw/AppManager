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
