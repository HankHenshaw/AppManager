#include "androidjni.h"
#include <QDebug>

AndroidJni::AndroidJni(QObject *parent)
    : QObject(parent)
{
    m_activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    Q_ASSERT(m_activity.isValid());
}

void AndroidJni::getPackageName()
{
    QAndroidJniObject context = QtAndroid::androidContext();

    QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");
    QAndroidJniObject lpi = pm.callObjectMethod("getInstalledPackages", "(I)Ljava/util/List;", 0); // 1048576 - MATCH_SYSTEM_ONLY

    jclass listClass = m_env.findClass("java/util/List");
    jmethodID sizeID = m_env->GetMethodID(listClass, "size", "()I");
    int size = static_cast<int>(m_env->CallIntMethod(lpi.object(), sizeID));
    qDebug() << "Size =" << size;

    jmethodID getId = m_env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
    jclass packageInfo = m_env.findClass("android/content/pm/PackageInfo");
    jfieldID packageNameID = m_env->GetFieldID(packageInfo, "packageName", "Ljava/lang/String;");

    for(int i = 0; i < size; ++i)
    {
        jobject obj = m_env->CallObjectMethod(lpi.object(), getId, i); // Package Info?
        QAndroidJniObject packName = m_env->GetObjectField(obj, packageNameID);
        if(!packName.isValid())
            qDebug() << "can't get packageName";
//      else
//          qDebug() << packName.toString();

        /*Compare if certain package system or not*/
        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");
        if(!pmClass)
            qDebug() << "can't find package manager class";
        jmethodID getAppInfoId = m_env->GetMethodID(pmClass, "getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;");
        if(!getAppInfoId)
            qDebug() << "can't get method getApplicationInfo";

        jstring packNameString = m_env->NewStringUTF(packName.toString().toStdString().c_str());
        if(!packNameString)
            qDebug() << "Can't create new jstring with package name";

        QAndroidJniObject appInfo = m_env->CallObjectMethod(pm.object(), getAppInfoId, packNameString, 0);
        if(!appInfo.isValid())
            qDebug() << "can't get appInfo";

        jclass appInfoClass = m_env.findClass("android/content/pm/ApplicationInfo");
        if(!appInfoClass)
            qDebug() << "Can't find app info class";

        jfieldID sourceDirId = m_env->GetFieldID(appInfoClass, "sourceDir", "Ljava/lang/String;");
        if(!sourceDirId)
            qDebug() << "Can't get sourceDir field Id";

        QAndroidJniObject sourceDir = m_env->GetObjectField(appInfo.object(), sourceDirId);
        if(!sourceDir.isValid())
            qDebug() << "can't get sourceDir field";

        jclass stringClass = m_env.findClass("java/lang/String");
        if(!stringClass)
            qDebug() << "Can't find jstring class";

        jmethodID startsWithId = m_env->GetMethodID(stringClass, "startsWith", "(Ljava/lang/String;)Z");
        if(!startsWithId)
            qDebug() << "Can't get starts with ID";

        jstring notSystemStr = m_env->NewStringUTF("/data/app/");
        if(!notSystemStr)
            qDebug() << "Can't create systemStr string";

        jboolean isNotSystemDir = m_env->CallBooleanMethod(sourceDir.object(), startsWithId, notSystemStr);
        if(isNotSystemDir)
            m_listOfPackName.append(packName.toString());
        /*Compare if packName system or not*/
    }
    m_listOfPackName.append("com.google.android.youtube"); //TODO: Remove
    qDebug() << "Pass through all functions";
}


void AndroidJni::getAppLabel()
{
    qDebug() << "Get App Label Method";
    QAndroidJniObject context = QtAndroid::androidContext();
    QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

    for(int i = 0; i < m_listOfPackName.size(); ++i)
    {
        jstring packageName = m_env->NewStringUTF(m_listOfPackName.at(i).toStdString().c_str());
        QAndroidJniObject AppInfo = pm.callObjectMethod("getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;", packageName, 128); // 128 - GET_META_DATA
        QAndroidJniObject appName = pm.callObjectMethod("getApplicationLabel", "(Landroid/content/pm/ApplicationInfo;)Ljava/lang/CharSequence;", AppInfo.object());

        m_listOfPackLabel.append(appName.toString());
    }
}

void AndroidJni::getAppIcon()
{
    QAndroidJniObject context = QtAndroid::androidContext();
    QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

    for(int i = 0; i < m_listOfPackName.size(); ++i)
    {
        jstring packageName = m_env->NewStringUTF(m_listOfPackName.at(i).toStdString().c_str());

        QAndroidJniObject icon = pm.callObjectMethod("getApplicationIcon", "(Ljava/lang/String;)Landroid/graphics/drawable/Drawable;", packageName);
        jclass bitmapDrawableClass = m_env.findClass("android/graphics/drawable/BitmapDrawable");
        jclass adaptiveIconDrawableClass = m_env.findClass("android/graphics/drawable/AdaptiveIconDrawable");
        if(m_env->IsInstanceOf(icon.object(), bitmapDrawableClass))
        {
            qDebug() << "Icon is bitmapDrawable class";

            jmethodID getBitmapId = m_env->GetMethodID(bitmapDrawableClass, "getBitmap", "()Landroid/graphics/Bitmap;");
            if(!getBitmapId)
                qDebug() << "Can't get getBitmap ID";

            QAndroidJniObject bitmap = m_env->CallObjectMethod(icon.object(), getBitmapId);
            if(!bitmap.isValid())
                qDebug() << "Can't get bitmap";

            /*Compress*/
            jclass bitmapClass = m_env.findClass("android/graphics/Bitmap");
            if(!bitmapClass)
                qDebug() << "Can't find Bitmap class";

            jclass jpegEnumClass = m_env.findClass("android/graphics/Bitmap$CompressFormat");

            jmethodID valueOfId = m_env->GetStaticMethodID(jpegEnumClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$CompressFormat;");
            jstring jpegString = m_env->NewStringUTF("JPEG");
            QAndroidJniObject enumValue = m_env->CallStaticObjectMethod(jpegEnumClass, valueOfId, jpegString);

            jclass baOutStreamClass = m_env.findClass("java/io/ByteArrayOutputStream");
            jmethodID initOutStream = m_env->GetMethodID(baOutStreamClass, "<init>", "()V");
            jobject outStream = m_env->NewObject(baOutStreamClass, initOutStream);

            jint imgQuality = 100;
            jmethodID compId = m_env->GetMethodID(bitmapClass, "compress", "(Landroid/graphics/Bitmap$CompressFormat;ILjava/io/OutputStream;)Z");

            jboolean isGood = m_env->CallBooleanMethod(bitmap.object(), compId, enumValue.object(), imgQuality, outStream);
            if(!isGood)
                qDebug() << "Compress didn't work";
            /*Compress*/
            /*jbyteArray to QByteArray*/
            jmethodID toByteArrId = m_env->GetMethodID(baOutStreamClass, "toByteArray", "()[B");
            jbyteArray byteArr = static_cast<jbyteArray>(m_env->CallObjectMethod(outStream, toByteArrId));

            int arrSize = m_env->GetArrayLength(byteArr);

            QByteArray iconArr;
            iconArr.resize(arrSize);
            m_env->GetByteArrayRegion(byteArr, 0, arrSize, reinterpret_cast<jbyte*>(iconArr.data()));
            /*jbyteArray to QByteArray*/

            m_icon.loadFromData(iconArr);
            if(m_icon.isNull())
                qDebug() << "BitmapDrawable Image is null";
            else
                qDebug() << "BitmapDrawable Image is not null";

            m_listOfPackIcon.append(m_icon);
        } else if (m_env->IsInstanceOf(icon.object(), adaptiveIconDrawableClass)) {
//            qDebug() << "Icon is adaptiveIconDrawable class";

            QAndroidJniObject iconFront = icon.callObjectMethod("getForeground", "()Landroid/graphics/drawable/Drawable;");
            QAndroidJniObject iconBack = icon.callObjectMethod("getBackground", "()Landroid/graphics/drawable/Drawable;");

            jclass drawableClass = m_env.findClass("android/graphics/drawable/Drawable");
            jobjectArray drawableArr = m_env->NewObjectArray(2, drawableClass, iconBack.object());

            m_env->SetObjectArrayElement(drawableArr, 1, iconFront.object());
            jclass layerDrawableClass = m_env.findClass("android/graphics/drawable/LayerDrawable");
            jmethodID layDrawableConstr = m_env->GetMethodID(layerDrawableClass, "<init>", "([Landroid/graphics/drawable/Drawable;)V");
            jobject layerDrawable = m_env->NewObject(layerDrawableClass, layDrawableConstr, drawableArr);

            jmethodID getWidthId = m_env->GetMethodID(layerDrawableClass, "getIntrinsicWidth", "()I");
            jmethodID getHeightId = m_env->GetMethodID(layerDrawableClass, "getIntrinsicHeight", "()I");

            jint iconWidth = m_env->CallIntMethod(layerDrawable, getWidthId);
            jint iconHeight = m_env->CallIntMethod(layerDrawable, getHeightId);

            jclass bitmapClass = m_env.findClass("android/graphics/Bitmap");
            jmethodID createBitmapId = m_env->GetStaticMethodID(bitmapClass, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

            jclass bitmapConfig = m_env.findClass("android/graphics/Bitmap$Config");
            jmethodID bitmapConfigValueOfID = m_env->GetStaticMethodID(bitmapConfig, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

            jstring valueARGB8888 = m_env->NewStringUTF("ARGB_8888");
            jobject valueOfARGB8888 = m_env->CallStaticObjectMethod(bitmapConfig, bitmapConfigValueOfID, valueARGB8888);

            jobject iconBitmap = m_env->CallStaticObjectMethod(bitmapClass, createBitmapId, iconWidth, iconHeight, valueOfARGB8888);

            jclass canvasClass = m_env.findClass("android/graphics/Canvas");
            jmethodID canvasInitId = m_env->GetMethodID(canvasClass, "<init>", "(Landroid/graphics/Bitmap;)V");
            jobject canvas = m_env->NewObject(canvasClass, canvasInitId, iconBitmap);

            jmethodID canvasWidthId = m_env->GetMethodID(canvasClass, "getWidth", "()I");
            jmethodID canvasHeightId = m_env->GetMethodID(canvasClass, "getHeight", "()I");
            jint canvasWidth = m_env->CallIntMethod(canvas, canvasWidthId);
            jint canvasHeight = m_env->CallIntMethod(canvas, canvasHeightId);

            jmethodID setBoundsId = m_env->GetMethodID(drawableClass, "setBounds", "(IIII)V");
            m_env->CallVoidMethod(layerDrawable, setBoundsId, 0, 0, canvasWidth, canvasHeight);

            jmethodID drawId = m_env->GetMethodID(layerDrawableClass, "draw", "(Landroid/graphics/Canvas;)V");
            m_env->CallVoidMethod(layerDrawable, drawId, canvas);

            /*Compress*/
            jclass jpegEnumClass = m_env.findClass("android/graphics/Bitmap$CompressFormat");

            jmethodID valueOfId = m_env->GetStaticMethodID(jpegEnumClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$CompressFormat;");
            jstring jpegString = m_env->NewStringUTF("JPEG");
            QAndroidJniObject enumValue = m_env->CallStaticObjectMethod(jpegEnumClass, valueOfId, jpegString);

            jclass baOutStreamClass = m_env.findClass("java/io/ByteArrayOutputStream");
            jmethodID initOutStream = m_env->GetMethodID(baOutStreamClass, "<init>", "()V");
            jobject outStream = m_env->NewObject(baOutStreamClass, initOutStream);

            jint imgQuality = 100;
            jmethodID compId = m_env->GetMethodID(bitmapClass, "compress", "(Landroid/graphics/Bitmap$CompressFormat;ILjava/io/OutputStream;)Z");

            jboolean isGood = m_env->CallBooleanMethod(iconBitmap, compId, enumValue.object(), imgQuality, outStream);
            if(!isGood)
                qDebug() << "Compress didn't work";
            /*Compress*/
            /*jbyteArray to QByteArray*/
            jmethodID toByteArrId = m_env->GetMethodID(baOutStreamClass, "toByteArray", "()[B");
            jbyteArray byteArr = static_cast<jbyteArray>(m_env->CallObjectMethod(outStream, toByteArrId));

            int arrSize = m_env->GetArrayLength(byteArr);

            QByteArray iconArr;
            iconArr.resize(arrSize);
            m_env->GetByteArrayRegion(byteArr, 0, arrSize, reinterpret_cast<jbyte*>(iconArr.data()));
            /*jbyteArray to QByteArray*/

            m_icon.loadFromData(iconArr);
//            if(m_icon.isNull())
//                qDebug() << "Image is null";
//            else
//                qDebug() << "Image is not null";

            m_listOfPackIcon.append(m_icon);
        } else {
            qDebug() << "Icon is another class";
        }
    }
}


QList<QString> AndroidJni::getListOfPackName() const
{
    return m_listOfPackName;
}

QList<QImage> AndroidJni::getListOfPackIcon() const
{
    return m_listOfPackIcon;
}

QList<QString> AndroidJni::getListOfPackLabel() const
{
    return m_listOfPackLabel;
}

void AndroidJni::slotRunApp(QVariant index)
{
    qDebug() << "Run app slot";
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        QString appQName = m_listOfPackName.at(index.toInt());

        jstring appName = m_env->NewStringUTF(appQName.toStdString().c_str());
        QAndroidJniObject launchIntent = pm.callObjectMethod("getLaunchIntentForPackage", "(Ljava/lang/String;)Landroid/content/Intent;", appName);

        jclass activityClass = m_env.findClass("android/app/Activity");
        jmethodID startActivityId = m_env->GetMethodID(activityClass, "startActivity", "(Landroid/content/Intent;)V");

        m_env->CallVoidMethod(context.object(), startActivityId, launchIntent.object());
    } else {
        qDebug() << "Invalid Index";
    }
}

void AndroidJni::slotAppInfo(QVariant index)
{
    qDebug() << "Run app slot";
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");
        jmethodID getAppInfoId = m_env->GetMethodID(pmClass, "getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;");
        if(!getAppInfoId)
            qDebug() << "can't get method getApplicationInfo";

        jclass appInfoClass = m_env.findClass("android/content/pm/ApplicationInfo");
        if(!appInfoClass)
            qDebug() << "Can't find app info class";

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        QAndroidJniObject appInfo = m_env->CallObjectMethod(pm.object(), getAppInfoId, packName, 0);
        if(!appInfo.isValid())
            qDebug() << "can't get appInfo";

        jfieldID categoryId = m_env->GetFieldID(appInfoClass, "category", "I");
        if(!categoryId)
            qDebug() << "Can't get Id of category field";

        jint category = m_env->GetIntField(appInfo.object(), categoryId);
        qDebug() << "Category: " << category;

        switch (category) {
            case Category::CATEGORY_GAME:
            {
                qDebug() << "Category Game";
                break;
            }
            case Category::CATEGORY_AUDIO:
            {
                qDebug() << "Category Audio";
                break;
            }
            case Category::CATEGORY_VIDEO:
            {
                qDebug() << "Category Video";
                break;
            }
            case Category::CATEGORY_IMAGE:
            {
                qDebug() << "Category Image";
                break;
            }
            case Category::CATEGORY_SOCIAL:
            {
                qDebug() << "Category Social";
                break;
            }
            case Category::CATEGORY_NEWS:
            {
                qDebug() << "Category News";
                break;
            }
            case Category::CATEGORY_MAPS:
            {
                qDebug() << "Category Maps";
                break;
            }
            case Category::CATEGORY_PRODUCTIVITY:
            {
                qDebug() << "Category Productivity";
                break;
            }
            case Category::CATEGORY_UNDEFINED:
            {
                qDebug() << "Category Undefined";
                break;
            }
            default:
            {
                qDebug() << "Undefined Number:" << category;
                break;
            }
        }



        /* Getting Permission */
        // TODO: Test and Move to separate slot/method
        jfieldID permissionId = m_env->GetFieldID(appInfoClass, "permission", "Ljava/lang/String;");
        if(!categoryId)
            qDebug() << "Can't get Id of permission field";

        QAndroidJniObject permissionString = m_env->GetObjectField(appInfo.object(), permissionId);
        qDebug() << "Permission:" << permissionString.toString();
        /* Getting Permission */

        /* Getting Min Sdk Version */
        //TODO: Сопоставить сдк версии, версию андроида https://source.android.com/setup/start/build-numbers
        jfieldID minSdkId = m_env->GetFieldID(appInfoClass, "minSdkVersion", "I");
        if(!minSdkId)
            qDebug() << "Can't get Id of minSdkVersion field";

        jint minSdkVersion = m_env->GetIntField(appInfo.object(), minSdkId);
        qDebug() << "Minimum Sdk Version:" << minSdkVersion;
        /* Getting Min Sdk Version */

        /* Getting Target Sdk Version */
        //TODO: Сопоставить сдк версии, версию андроида https://source.android.com/setup/start/build-numbers
        jfieldID targetSdkId = m_env->GetFieldID(appInfoClass, "targetSdkVersion", "I");
        if(!targetSdkId)
            qDebug() << "Can't get Id of targetSdkVersion field";

        jint targetSdkVersion = m_env->GetIntField(appInfo.object(), targetSdkId);
        qDebug() << "Target Sdk Version:" << targetSdkVersion;
        /* Getting Target Sdk Version */

        /* Getting Full path to the base Apk*/
        jfieldID sourceId = m_env->GetFieldID(appInfoClass, "sourceDir", "Ljava/lang/String;");
        if(!sourceId)
            qDebug() << "Can't get Id of sourceDir field";

        QAndroidJniObject sourceDir = m_env->GetObjectField(appInfo.object(), sourceId);
        qDebug() << "Path:" << sourceDir.toString();
        /* Getting Full path to the base Apk*/

        /* Getting App Version */
        jmethodID getPackageInfoId = m_env->GetMethodID(pmClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        if(!getPackageInfoId)
            qDebug() << "Can't get getPackageInfoId method";

        QAndroidJniObject packageInfo = m_env->CallObjectMethod(pm.object(), getPackageInfoId, packName, 0);
        if(!packageInfo.isValid())
            qDebug() << "Package Info is invalid";

        jclass packageInfoClass = m_env.findClass("android/content/pm/PackageInfo");

        jfieldID versionNameId = m_env->GetFieldID(packageInfoClass, "versionName", "Ljava/lang/String;");
        if(!versionNameId)
            qDebug() << "Can't get id of versionName field";

        QAndroidJniObject version = m_env->GetObjectField(packageInfo.object(), versionNameId);
        qDebug() << "Version:" << version.toString();
        /* Getting App Version */
    } else {
        qDebug() << "Invalid Index";
    }
}
