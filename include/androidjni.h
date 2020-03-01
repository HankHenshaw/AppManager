#ifndef ANDROIDJNI_H
#define ANDROIDJNI_H

#include <QObject>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QVariant>

class AndroidJni : public QObject
{
    Q_OBJECT
public:
    explicit AndroidJni(QObject *parent = nullptr);
    void getPackageName();
    void getAppLabel();
    void getAppIcon();

    enum Category {CATEGORY_UNDEFINED = -1,
                   CATEGORY_GAME,
                   CATEGORY_AUDIO,
                   CATEGORY_VIDEO,
                   CATEGORY_IMAGE,
                   CATEGORY_SOCIAL,
                   CATEGORY_NEWS,
                   CATEGORY_MAPS,
                   CATEGORY_PRODUCTIVITY};

    QList<QString> getListOfPackName() const;
    QList<QImage> getListOfPackIcon() const;
    QList<QString> getListOfPackLabel() const;

    QVariant getCategory(QVariant index);
    QVariant getVersion(QVariant index);
    QVariant getMinSdk(QVariant index);
    QVariant getTargetSdk(QVariant index);
    QVariant getPackagePath(QVariant index);
    QVariant getPackageName(QVariant index);
    QVariant getPackageSize(QVariant index);
    QVariant getCacheSize(QVariant index);
    QVariant getRequestedPermissions(QVariant index);
    QVariant getFirstInstallationTime(QVariant index);
    QVariant getLastUpdateTime(QVariant index);
    void deleteApp(QVariant index);
    void clearCache(QVariant index);
    QVariant getPermissionsNumber();
    bool isSystemApp(int index);
    uint getNumberOfNonSystemApp() const;

private:
    QList<QString> m_listOfPackName;
    QList<QImage> m_listOfPackIcon;
    QList<QString> m_listOfPackLabel;
    QList<QString> m_listOfSysApps; // TODO
    QImage m_icon;
    int m_numberOfPermissions;
    uint m_numberOfNonSystemApp;
    //JNI Objects
    QAndroidJniEnvironment m_env;
    QAndroidJniObject m_activity;

    jlong sizeOfFiles(const QAndroidJniObject& obj);
    jboolean deleteDir(QAndroidJniObject& dir);
public slots:
    void slotRunApp(QVariant index);
    void slotAppInfo(QVariant index);
};

#endif // ANDROIDJNI_H
