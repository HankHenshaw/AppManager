#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "imageprovider.h"
#include "androidjni.h"

class AppModel : public QAbstractListModel
{
    Q_OBJECT
public:
    AppModel(QObject *parent = nullptr);
    AppModel(ImageProvider *ip, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
private:
    ImageProvider *m_imgProvider;
    AndroidJni m_native;
    QList<QString> m_nameList;
signals:
    void sigRunApp(QVariant index);
};

#endif // APPMODEL_H
