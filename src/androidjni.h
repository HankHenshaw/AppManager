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

    QList<QString> getListOfPackName() const;
    QList<QImage> getListOfPackIcon() const;
    QList<QString> getListOfPackLabel() const;
private:
    QList<QString> m_listOfPackName;
    QList<QImage> m_listOfPackIcon;
    QList<QString> m_listOfPackLabel;
    QList<QString> m_listOfSysApps; // TODO
    QImage m_icon;
    //JNI Objects
    QAndroidJniEnvironment m_env;
    QAndroidJniObject m_activity;
public slots:
    void slotRunApp(QVariant index);
};

#endif // ANDROIDJNI_H
