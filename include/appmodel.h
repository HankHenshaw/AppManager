#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QTranslator>
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
    Q_INVOKABLE QVariant getCategory(QVariant index);
    Q_INVOKABLE QVariant getVersion(QVariant index);
    Q_INVOKABLE QVariant getMinSdk(QVariant index);
    Q_INVOKABLE QVariant getTargetSdk(QVariant index);
    Q_INVOKABLE QVariant getPackagePath(QVariant index);
    Q_INVOKABLE QVariant getPackageName(QVariant index);
    Q_INVOKABLE QVariant getPackageSize(QVariant index);
    Q_INVOKABLE QVariant getCacheSize(QVariant index);
    Q_INVOKABLE QVariant getRequestedPermissions(QVariant index);
    Q_INVOKABLE QVariant getPermissionsNumber();
    Q_INVOKABLE QVariant getFirstInstallationTime(QVariant index);
    Q_INVOKABLE QVariant getLastUpdateTime(QVariant index);
    Q_INVOKABLE void deleteApp(QVariant index);
    Q_INVOKABLE void clearCache(QVariant index);
    Q_INVOKABLE void switchChanged(bool state);
    Q_INVOKABLE void changeLanguage(bool isEnglish);
private:
    ImageProvider *m_imgProvider;
    mutable AndroidJni m_native;
    QList<QString> m_nameList;
    bool isShowAllApps;
    QTranslator *translator;
signals:
    void sigRunApp(QVariant index);
    void signalAppInfo(QVariant index);
};

#endif // APPMODEL_H
