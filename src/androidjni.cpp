#include "androidjni.h"
#include <QDebug>
#include <QSysInfo>

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

QVariant AndroidJni::getCategory(QVariant index)
{
    QString categoryStr;
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
                categoryStr = "Game";
                qDebug() << "Category Game";
                break;
            }
            case Category::CATEGORY_AUDIO:
            {
                categoryStr = "Audio";
                qDebug() << "Category Audio";
                break;
            }
            case Category::CATEGORY_VIDEO:
            {
                categoryStr = "Video";
                qDebug() << "Category Video";
                break;
            }
            case Category::CATEGORY_IMAGE:
            {
                categoryStr = "Image";
                qDebug() << "Category Image";
                break;
            }
            case Category::CATEGORY_SOCIAL:
            {
                categoryStr = "Social";
                qDebug() << "Category Social";
                break;
            }
            case Category::CATEGORY_NEWS:
            {
                categoryStr = "News";
                qDebug() << "Category News";
                break;
            }
            case Category::CATEGORY_MAPS:
            {
                categoryStr = "Maps";
                qDebug() << "Category Maps";
                break;
            }
            case Category::CATEGORY_PRODUCTIVITY:
            {
                categoryStr = "Productivity";
                qDebug() << "Category Productivity";
                break;
            }
            case Category::CATEGORY_UNDEFINED:
            {
                categoryStr = "Undefined";
                qDebug() << "Category Undefined";
                break;
            }
            default:
            {
                qDebug() << "Undefined Number:" << category;
                break;
            }
        }
    }
    return categoryStr;
}

QVariant AndroidJni::getVersion(QVariant index)
{
    QString versionStr;

    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");
        jmethodID getAppInfoId = m_env->GetMethodID(pmClass, "getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;");
        if(!getAppInfoId)
            qDebug() << "can't get method getApplicationInfo";

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

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

        versionStr = version.toString();
    }

    return versionStr;
}

QVariant AndroidJni::getMinSdk(QVariant index) //TODO: Convert API vers to Android vers
{
    int minSdk = -1;
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

        jfieldID minSdkId = m_env->GetFieldID(appInfoClass, "minSdkVersion", "I");
        if(!minSdkId)
            qDebug() << "Can't get Id of minSdkVersion field";

        jint minSdkVersion = m_env->GetIntField(appInfo.object(), minSdkId);
        qDebug() << "Minimum Sdk Version:" << minSdkVersion;

        minSdk = minSdkVersion;
    }
    return minSdk;
}

QVariant AndroidJni::getTargetSdk(QVariant index) //TODO: Convert API vers to Android vers
{
    int targetSdk = -1;
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

        jfieldID targetSdkId = m_env->GetFieldID(appInfoClass, "targetSdkVersion", "I");
        if(!targetSdkId)
            qDebug() << "Can't get Id of targetSdkVersion field";

        jint targetSdkVersion = m_env->GetIntField(appInfo.object(), targetSdkId);
        qDebug() << "Target Sdk Version:" << targetSdkVersion;

        targetSdk = targetSdkVersion;
    }
    return targetSdk;
}

QVariant AndroidJni::getPackagePath(QVariant index)
{
    QString pathStr;
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

        jfieldID sourceId = m_env->GetFieldID(appInfoClass, "sourceDir", "Ljava/lang/String;");
        if(!sourceId)
            qDebug() << "Can't get Id of sourceDir field";

        QAndroidJniObject sourceDir = m_env->GetObjectField(appInfo.object(), sourceId);
        qDebug() << "Path:" << sourceDir.toString();

        pathStr = sourceDir.toString();
    }
    return pathStr;
}

QVariant AndroidJni::getPackageName(QVariant index)
{
    if(index >= 0 && index < m_listOfPackName.size())
    {
        return m_listOfPackName.at(index.toInt());
    }
    return "Invalid Index";
}

QVariant AndroidJni::getPackageSize(QVariant index)
{
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

        jfieldID sourceId = m_env->GetFieldID(appInfoClass, "sourceDir", "Ljava/lang/String;");
        if(!sourceId)
            qDebug() << "Can't get Id of sourceDir field";

        QAndroidJniObject sourceDir = m_env->GetObjectField(appInfo.object(), sourceId);
        qDebug() << "Path:" << sourceDir.toString();

        jclass fileClass = m_env.findClass("java/io/File");
        if(!fileClass)
            qDebug() << "Failed to find File class";

        jmethodID fileConstrId = m_env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        if(!fileConstrId)
            qDebug() << "Failed to get id of File constructor";

        QAndroidJniObject file = m_env->NewObject(fileClass, fileConstrId, sourceDir.object());
        if(!file.isValid())
            qDebug() << "Failed to create File object";

        jmethodID lengthId = m_env->GetMethodID(fileClass, "length", "()J");
        if(!lengthId)
            qDebug() << "Failed to get id of length method";

        jlong dirSize = m_env->CallLongMethod(file.object(), lengthId);

        int size = dirSize/1048576;
        return size;
    }
    return 0;
}

QVariant AndroidJni::getCacheSize(QVariant index) // TODO: Директори кэша определяется не правильно, берется папка для текущего приложения
{
    int cache = 0;
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        jclass fileClass = m_env.findClass("java/io/File");
        if(!fileClass)
            qDebug() << "Failed to find File class";

        /*Getting App Cache Dir Size*/
        jclass contextClass = m_env.findClass("android/content/Context");
        if(!contextClass)
            qDebug() << "Failed to get Context class";

        jmethodID createPackageContextId = m_env->GetMethodID(contextClass, "createPackageContext", "(Ljava/lang/String;I)Landroid/content/Context;");
        if(!createPackageContextId)
            qDebug() << "Failed to get id of createPackageContext method";

        QAndroidJniObject packContext = m_env->CallObjectMethod(context.object(), createPackageContextId, packName, 2);
        if(!packContext.isValid())
            qDebug() << "Failed to createPackageContext";

        /*Cache Dir*/
        jmethodID getCacheDirId = m_env->GetMethodID(contextClass, "getCacheDir", "()Ljava/io/File;");
        if(!getCacheDirId)
            qDebug() << "Failed to get id of getCacheDir method";

        QAndroidJniObject cacheFile = m_env->CallObjectMethod(context.object(), getCacheDirId);
        if(!cacheFile.isValid())
            qDebug() << "Failed to get cache file";

        qDebug() << "Cache dir:" << cacheFile.toString();
        /*Cache Dir*/
        /*External Cache Dir*/
        jmethodID getExternalCacheDirId = m_env->GetMethodID(contextClass, "getExternalCacheDir", "()Ljava/io/File;");
        if(!getExternalCacheDirId)
            qDebug() << "Failed to get id of getExternalCacheDir method";

        QAndroidJniObject externalCacheFile = m_env->CallObjectMethod(context.object(), getExternalCacheDirId);
        if(!externalCacheFile.isValid())
            qDebug() << "Failed to get path of externalChacheFile";

        qDebug() << "External Cache dir:" << externalCacheFile.toString();
        /*External Cache Dir*/
        /*Getting List of Cache Files*/
        jmethodID listFilesId = m_env->GetMethodID(fileClass, "listFiles", "()[Ljava/io/File;");
        if(!listFilesId)
            qDebug() << "Failed to get id of listFiles method";

        QAndroidJniObject fileArray = m_env->CallObjectMethod(cacheFile.object(), listFilesId);
        if(!fileArray.isValid())
            qDebug() << "Failed to get array of cache directory";

        /*Getting List of Cache Files*/
        /*Getting App Cache Dir Size*/
        jlong cacheBytes = 0;
        cacheBytes += sizeOfFiles(fileArray);
        //TODO!
        /*Getting App Cache Dir Size*/
        /*Getting App External Cache Dir Size*/
        QAndroidJniObject externalFileArray = m_env->CallObjectMethod(externalCacheFile.object(), listFilesId);
        if(!externalFileArray.isValid())
            qDebug() << "Failed to get list of external cache files";

        cacheBytes += sizeOfFiles(externalFileArray);
        cache = cacheBytes/1024;
    }
    return cache;
}

QVariant AndroidJni::getRequestedPermissions(QVariant index)
{
    QString permString;
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        jclass packageInfoClass = m_env.findClass("android/content/pm/PackageInfo");

        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");

        jmethodID getPackageInfoId = m_env->GetMethodID(pmClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        if(!getPackageInfoId)
            qDebug() << "Can't get getPackageInfoId method";

        QAndroidJniObject packageInfoPermission = m_env->CallObjectMethod(pm.object(), getPackageInfoId, packName, 4096);
        if(!packageInfoPermission.isValid())
            qDebug() << "Package Info is invalid";

        jfieldID requestedPermissionId = m_env->GetFieldID(packageInfoClass, "requestedPermissions", "[Ljava/lang/String;");
        if(!requestedPermissionId)
            qDebug() << "Can't get id of requestedPermissions field";

        QAndroidJniObject requestedPermission = m_env->GetObjectField(packageInfoPermission.object(), requestedPermissionId);
        if(!requestedPermission.isValid())
            qDebug() << "Failed to get requestedPermissions";  //Возможно что здесь фейлится из-за того что запускаю на симуляторе

        jint permissionArrSize = m_env->GetArrayLength(static_cast<jobjectArray>(requestedPermission.object()));
        qDebug() << "Number of permissions:" << permissionArrSize;

        m_numberOfPermissions = permissionArrSize;

        jobjectArray permArray = static_cast<jobjectArray>(requestedPermission.object());
        for(int i = 0; i < permissionArrSize; ++i)
        {
            jsize index = i;
            QAndroidJniObject arrElem = m_env->GetObjectArrayElement(permArray, index);
            QString permisString = arrElem.toString();
            int counter = 0;
            int chars = 0;
            for(int j = 0; j < permisString.size(); ++j)
            {
                ++counter;
                if(permisString.at(j) == '.') chars = counter;
            }
            if(chars) permisString.remove(0, chars);
            permString += permisString + '\n';
        }
        permString.remove(permString.size()-1, 1);
    }
    return permString;
}

QVariant AndroidJni::getFirstInstallationTime(QVariant index)
{
    QString firstInstallationTime;
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");

        jmethodID getPackageInfoId = m_env->GetMethodID(pmClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        if(!getPackageInfoId)
            qDebug() << "Can't get getPackageInfoId method";

        QAndroidJniObject packageInfo = m_env->CallObjectMethod(pm.object(), getPackageInfoId, packName, 0);
        if(!packageInfo.isValid())
            qDebug() << "Package Info is invalid";

        jclass packageInfoClass = m_env.findClass("android/content/pm/PackageInfo");

        jfieldID firstInstTimeId = m_env->GetFieldID(packageInfoClass, "firstInstallTime", "J");
        if(!firstInstTimeId)
            qDebug() << "Failed to get id of firstInstallationTime field";

        jlong firstInstallTime = m_env->GetLongField(packageInfo.object(), firstInstTimeId);
        qDebug() << "firstInstTime:" << firstInstallTime;

        jclass dateClass = m_env.findClass("java/util/Date");
        if(!dateClass)
            qDebug() << "Can't find date class";

        jclass simpleDateFormatClass = m_env.findClass("java/text/SimpleDateFormat");
        if(!simpleDateFormatClass)
            qDebug() << "Can't find simpleDateFormat class";

        jclass formatClass = m_env.findClass("java/text/Format");
        if(!formatClass)
            qDebug() << "Can't find format class";

        jmethodID dateInitId = m_env->GetMethodID(dateClass, "<init>", "(J)V");
        if(!dateInitId)
            qDebug() << "Failed to get id of date constructor";

        jmethodID simpleFormatInitId = m_env->GetMethodID(simpleDateFormatClass, "<init>", "(Ljava/lang/String;)V");
        if(!simpleFormatInitId)
            qDebug() << "Failed to get id of simpleFormat constructor";

        jobject installDate = m_env->NewObject(dateClass, dateInitId, firstInstallTime);
        if(!installDate)
            qDebug() << "Failed to create new date file";

        jstring dateFormatStr = m_env->NewStringUTF("yyyy-MM-dd HH:mm:ss");
        if(!dateFormatStr)
            qDebug() << "Failed to create new string";

        jobject timeFormat = m_env->NewObject(simpleDateFormatClass, simpleFormatInitId, dateFormatStr);
        if(!timeFormat)
            qDebug() << "Failed to create new format object";

        jmethodID formatId = m_env->GetMethodID(formatClass, "format", "(Ljava/lang/Object;)Ljava/lang/String;");
        if(!formatId)
            qDebug() << "Failed to get id of format method id";

        QAndroidJniObject firstInstallDateStr = m_env->CallObjectMethod(timeFormat, formatId, installDate);
        if(!firstInstallDateStr.isValid())
            qDebug() << "Install Date string isn't valid";
        else
            firstInstallationTime = firstInstallDateStr.toString();
    }
    return firstInstallationTime;
}

QVariant AndroidJni::getLastUpdateTime(QVariant index)
{
    QString lastUpdateTimeStr;
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        jclass pmClass = m_env.findClass("android/content/pm/PackageManager");

        jmethodID getPackageInfoId = m_env->GetMethodID(pmClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        if(!getPackageInfoId)
            qDebug() << "Can't get getPackageInfoId method";

        QAndroidJniObject packageInfo = m_env->CallObjectMethod(pm.object(), getPackageInfoId, packName, 0);
        if(!packageInfo.isValid())
            qDebug() << "Package Info is invalid";

        jclass packageInfoClass = m_env.findClass("android/content/pm/PackageInfo");

        jfieldID lastUpdateTimeId = m_env->GetFieldID(packageInfoClass, "lastUpdateTime", "J");
        if(!lastUpdateTimeId)
            qDebug() << "Failed to get id of lastUpdateTime field";

        jlong lastUpdateTime = m_env->GetLongField(packageInfo.object(), lastUpdateTimeId);
        qDebug() << "lastUpdTime:" << lastUpdateTime;

        jclass dateClass = m_env.findClass("java/util/Date");
        if(!dateClass)
            qDebug() << "Can't find date class";

        jclass simpleDateFormatClass = m_env.findClass("java/text/SimpleDateFormat");
        if(!simpleDateFormatClass)
            qDebug() << "Can't find simpleDateFormat class";

        jclass formatClass = m_env.findClass("java/text/Format");
        if(!formatClass)
            qDebug() << "Can't find format class";

        jmethodID dateInitId = m_env->GetMethodID(dateClass, "<init>", "(J)V");
        if(!dateInitId)
            qDebug() << "Failed to get id of date constructor";

        jmethodID simpleFormatInitId = m_env->GetMethodID(simpleDateFormatClass, "<init>", "(Ljava/lang/String;)V");
        if(!simpleFormatInitId)
            qDebug() << "Failed to get id of simpleFormat constructor";

        jobject lastUpdateDate = m_env->NewObject(dateClass, dateInitId, lastUpdateTime);
        if(!lastUpdateDate)
            qDebug() << "Failed to create new date file";

        jstring dateFormatStr = m_env->NewStringUTF("yyyy-MM-dd HH:mm:ss");
        if(!dateFormatStr)
            qDebug() << "Failed to create new string";

        jobject timeFormat = m_env->NewObject(simpleDateFormatClass, simpleFormatInitId, dateFormatStr);
        if(!timeFormat)
            qDebug() << "Failed to create new format object";

        jmethodID formatId = m_env->GetMethodID(formatClass, "format", "(Ljava/lang/Object;)Ljava/lang/String;");
        if(!formatId)
            qDebug() << "Failed to get id of format method id";

        QAndroidJniObject lastUpdateDateStr = m_env->CallObjectMethod(timeFormat, formatId, lastUpdateDate);
        if(!lastUpdateDateStr.isValid())
            qDebug() << "Install Date string isn't valid";
        else
            lastUpdateTimeStr = lastUpdateDateStr.toString();
    }
    return lastUpdateTimeStr;
}

void AndroidJni::deleteApp(QVariant index)
{
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        QString appQName = m_listOfPackName.at(index.toInt());

        uint lolliPopVersion = 5;
        uint version = QSysInfo::productVersion().toFloat();
        if(version < lolliPopVersion) //TODO: Check this
        {
            qDebug() << "Current version" << version << "less than 5.0 (LolliPop)";
            jclass runtimeClass = m_env.findClass("java/lang/Runtime");
            if(!runtimeClass)
                qDebug() << "can't find runtime class";

            jmethodID getRuntimeId = m_env->GetStaticMethodID(runtimeClass, "getRuntime", "()Ljava/lang/Runtime;");
            if(!getRuntimeId)
                qDebug() << "failed to get id of getRuntime method";

            jobject runtimeObj = m_env->CallStaticObjectMethod(runtimeClass, getRuntimeId);
            if(!runtimeObj)
                qDebug() << "Failed to get runtime obj";

            QString cmdStr("pm clear " + appQName);
            jstring cmdJstr = m_env->NewStringUTF(cmdStr.toStdString().c_str());

            jmethodID execId = m_env->GetMethodID(runtimeClass, "exec", "(Ljava/lang/String;)Ljava/lang/Process;");
            if(!execId)
                qDebug() << "Failed to get id of exec method";

            m_env->CallObjectMethod(runtimeObj, execId, cmdJstr);
        } else {
            qDebug() << "Current version" << version << "equal or greater than 5.0 (LolliPop)";

            jstring activityServiceStr = m_env->NewStringUTF("activity");

            QAndroidJniObject activityManager = context.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", activityServiceStr);
            if(!activityManager.isValid())
                qDebug() << "Failed to invoke getSystemService method";

            jclass activityManagerClass = m_env.findClass("android/app/ActivityManager");
            if(!activityManagerClass)
                qDebug() << "failed to find activityManager class";

            jmethodID clearAppUserDataId = m_env->GetMethodID(activityManagerClass, "clearApplicationUserData", "()Z");
            if(!clearAppUserDataId)
                qDebug() << "Failed to get id of clearApplicationUserData method";

            qDebug() << "Good";
            jboolean isDeletionSuccess = m_env->CallBooleanMethod(activityManager.object(), clearAppUserDataId);
            if(isDeletionSuccess)
            {
                qDebug() << "Deletion succeed";
            } else {
                qDebug() << "Deletion failed";
            }
        }
    }
}

void AndroidJni::clearCache(QVariant index) // TODO: Директори кэша определяется не правильно, берется папка для текущего приложения
{
    if(index >= 0 && index < m_listOfPackName.size())
    {
        QAndroidJniObject context = QtAndroid::androidContext();
        QAndroidJniObject pm = context.callObjectMethod("getPackageManager", "()Landroid/content/pm/PackageManager;");

        jstring packName = m_env->NewStringUTF(m_listOfPackName.at(index.toInt()).toStdString().c_str());
        if(!packName) qDebug() << "Can't create new string";

        jclass fileClass = m_env.findClass("java/io/File");
        if(!fileClass)
            qDebug() << "Failed to find File class";

        /*Getting App Cache Dir Size*/
        jclass contextClass = m_env.findClass("android/content/Context");
        if(!contextClass)
            qDebug() << "Failed to get Context class";

        jmethodID createPackageContextId = m_env->GetMethodID(contextClass, "createPackageContext", "(Ljava/lang/String;I)Landroid/content/Context;");
        if(!createPackageContextId)
            qDebug() << "Failed to get id of createPackageContext method";

        QAndroidJniObject packContext = m_env->CallObjectMethod(context.object(), createPackageContextId, packName, 2);
        if(!packContext.isValid())
            qDebug() << "Failed to createPackageContext";

        /*Cache Dir*/
        jmethodID getCacheDirId = m_env->GetMethodID(contextClass, "getCacheDir", "()Ljava/io/File;");
        if(!getCacheDirId)
            qDebug() << "Failed to get id of getCacheDir method";

        QAndroidJniObject cacheFile = m_env->CallObjectMethod(context.object(), getCacheDirId);
        if(!cacheFile.isValid())
            qDebug() << "Failed to get cache file";

        qDebug() << "Cache dir:" << cacheFile.toString();
    }
}

QVariant AndroidJni::getPermissionsNumber()
{
    return m_numberOfPermissions;
}

jlong AndroidJni::sizeOfFiles(const QAndroidJniObject &obj)
{
    jclass fileClass = m_env.findClass("java/io/File"); //TODO: REFACTOR
    if(!fileClass)
        qDebug() << "Can't find File class";

    jmethodID isDirectoryId = m_env->GetMethodID(fileClass, "isDirectory", "()Z");
    if(!isDirectoryId)
        qDebug() << "Failed to get id of isDirectory method";

    jmethodID isFileId = m_env->GetMethodID(fileClass, "isFile", "()Z");
    if(!isFileId)
        qDebug() << "Failed to get id of isFile method";

    jmethodID lengthId = m_env->GetMethodID(fileClass, "length", "()J");
    if(!lengthId)
        qDebug() << "Failed to get id of length method";

    jmethodID listFilesId = m_env->GetMethodID(fileClass, "listFiles", "()[Ljava/io/File;");
    if(!listFilesId)
        qDebug() << "Failed to get id of listFiles method";

    jint arrSize = m_env->GetArrayLength(static_cast<jobjectArray>(obj.object()));
    jobjectArray fileArray = static_cast<jobjectArray>(obj.object());
    jlong sum = 0;
    qDebug() << "Cache Array Size:" << arrSize;

    for(int i = 0; i < arrSize; ++i)
    {
        jsize index = static_cast<jint>(i);
        QAndroidJniObject arrElem = m_env->GetObjectArrayElement(fileArray, index);
        if(!arrElem.isValid())
            qDebug() << "Failed to get element of array";

        jboolean isDirectory = m_env->CallBooleanMethod(arrElem.object(), isDirectoryId);
        jboolean isFile = m_env->CallBooleanMethod(arrElem.object(), isFileId);

        if(arrElem.isValid() && isDirectory)
        {
            QAndroidJniObject arr = m_env->CallObjectMethod(arrElem.object(), listFilesId);
            sum += sizeOfFiles(arr);
        } else if(arrElem.isValid() && isFile) {
            sum += m_env->CallLongMethod(arrElem.object(), lengthId);
        }
    }
    return sum;
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

/*Remove Later*/
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

        /* Getting Requested Permission */
        QAndroidJniObject packageInfoPermission = m_env->CallObjectMethod(pm.object(), getPackageInfoId, packName, 4096);
        if(!packageInfoPermission.isValid())
            qDebug() << "Package Info is invalid";

        jfieldID requestedPermissionId = m_env->GetFieldID(packageInfoClass, "requestedPermissions", "[Ljava/lang/String;");
        if(!requestedPermissionId)
            qDebug() << "Can't get id of requestedPermissions field";

        QAndroidJniObject requestedPermission = m_env->GetObjectField(packageInfoPermission.object(), requestedPermissionId);
        if(!requestedPermission.isValid())
            qDebug() << "Failed to get requestedPermissions";  //Возможно что здесь фейлится из-за того что запускаю на симуляторе

        jint permissionArrSize = m_env->GetArrayLength(static_cast<jobjectArray>(requestedPermission.object()));
        qDebug() << "Number of permissions:" << permissionArrSize;

        jobjectArray permArray = static_cast<jobjectArray>(requestedPermission.object());
        for(int i = 0; i < permissionArrSize; ++i)
        {
            jsize index = i;
            QAndroidJniObject arrElem = m_env->GetObjectArrayElement(permArray, index);
            qDebug() << "Permission:" << arrElem.toString();
        }
        /* Getting Requested Permission */

        /* Getting package size */
        //Not necessary?
//        jfieldID publicSourceDirId = m_env->GetFieldID(appInfoClass, "publicSourceDir", "Ljava/lang/String;");
//        if(!publicSourceDirId)
//            qDebug() << "Can't get id of publicSourceDir field";

//        QAndroidJniObject publicSourceDir = m_env->GetObjectField(appInfo.object(), publicSourceDirId);
//        qDebug() << publicSourceDir.toString();
        //Not neccessary?

//        jclass methodClass = m_env.findClass("java.lang.reflect.Method");
//        if(!methodClass)
//            qDebug() << "Can't find method class";

        /* For Api < 26 */
        jmethodID getClassId = m_env->GetMethodID(pmClass, "getClass", "()Ljava/lang/Class;");
        if(!getClassId)
            qDebug() << "Can't get id of getClass method";

        QAndroidJniObject classObj = m_env->CallObjectMethod(pm.object(), getClassId);
        if(!classObj.isValid())
            qDebug() << "Can't get class obj";

        jclass classClass = m_env.findClass("java/lang/Class");
        if(!classClass)
            qDebug() << "Can't find Class class";

        //TODO!
        /* For Api < 26 */
        /* For Api >= 26 */
        jstring storageStatsService = m_env->NewStringUTF("storagestats");
        if(!storageStatsService)
            qDebug() << "Can't create new string";

        QAndroidJniObject storageStatsManager = context.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", storageStatsService);
        if(!storageStatsManager.isValid())
            qDebug() << "Can't get StorageStatsManager from getSystemService method";

        jstring storageService = m_env->NewStringUTF("storage");
        if(!storageService)
            qDebug() << "Can't create new string";

        QAndroidJniObject storageManager = context.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", storageService);
        if(!storageManager.isValid())
            qDebug() << "Can't get StorageManager from getSystemService method";

        jclass storageStatsClass = m_env.findClass("android/app/usage/StorageStats");
        if(!storageStatsClass)
            qDebug() << "Can't find storageStats class";

        jclass storageStatsManagerClass = m_env.findClass("android/app/usage/StorageStatsManager");
        if(!storageStatsManagerClass)
            qDebug() << "Can't find storageStatsManager class";

        jfieldID storageUuidId = m_env->GetFieldID(appInfoClass, "storageUuid", "Ljava/util/UUID;");
        if(!storageUuidId)
            qDebug() << "Can't get id of storageUuid field";

        jfieldID uidId = m_env->GetFieldID(appInfoClass, "uid", "I");
        if(!uidId)
            qDebug() << "Can't get id of uid field";

        jint uid = m_env->GetIntField(appInfo.object(), uidId);
        if(!uid)
            qDebug() << "Failed to get uid";

        QAndroidJniObject storageUuid = m_env->GetObjectField(appInfo.object(), storageUuidId);
        if(!storageUuid.isValid())
            qDebug() << "Failed to get storageUuid";

        jmethodID queryStatsForUidId = m_env->GetMethodID(storageStatsManagerClass, "queryStatsForUid", "(Ljava/util/UUID;I)Landroid/app/usage/StorageStats;");
        if(!queryStatsForUidId)
            qDebug() << "Can't get id of queryStatsForUid method";

//        QAndroidJniObject storageStats = m_env->CallObjectMethod(storageStatsManager.object(), queryStatsForUidId, storageUuid.object(), uid);
//        if(!storageStats.isValid())
//            qDebug() << "Failed to get storageStats";
        //TODO: storageStats failed
        //TODO: Looking for queryStatsForPackage method instead of queryStatsForUid
        //TODO: also Looking for USAGE ACCESS PERMISSION
//        jclass contextClass = m_env.findClass("android/content/Context");
//        if(!contextClass)
//            qDebug() << "Can't find Context class";

//        jmethodID getSystemServiceId = m_env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
//        if(!getSystemServiceId)
//            qDebug() << "Can't get id of getSystemService method";

//        jstring appOpServiceStr = m_env->NewStringUTF("appops");
//        if(!appOpServiceStr)
//            qDebug() << "Failed to create new string";

//        QAndroidJniObject getSystemService = m_env->CallObjectMethod(context.object(), getSystemServiceId, appOpServiceStr);
//        if(!getSystemService.isValid())
//            qDebug() << "Failed to get SystemService";

//        jstring usageAccessSettingsStr = m_env->NewStringUTF("android.settings.USAGE_ACCESS_SETTINGS");
//        if(!usageAccessSettingsStr)
//            qDebug() << "Failed to create new string";

//        jclass intentClass = m_env.findClass("android/content/Intent");
//        if(!intentClass)
//            qDebug() << "Can't find Intent class";

//        jmethodID intentConstrId = m_env->GetMethodID(intentClass, "<init>", "(Ljava/lang/String;)V");
//        if(!intentConstrId)
//            qDebug() << "Can't get id of intent constructor";

//        QAndroidJniObject intent = m_env->NewObject(intentClass, intentConstrId, usageAccessSettingsStr);
//        if(!intent.isValid())
//            qDebug() << "Failed to create intent object";

//        jclass activityClass = m_env.findClass("android/app/Activity");
//        if(!activityClass)
//            qDebug() << "Can't find Activity class";

//        jmethodID startActivityId = m_env->GetMethodID(activityClass, "startActivity", "(Landroid/content/Intent;)V");
//        if(!startActivityId)
//            qDebug() << "Failed to get id of startActivity method";

//        m_env->CallVoidMethod(context.object(), startActivityId, intent.object());
        /*Getting App Size*/
        jclass fileClass = m_env.findClass("java/io/File");
        if(!fileClass)
            qDebug() << "Failed to find File class";

        jmethodID fileConstrId = m_env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;)V");
        if(!fileConstrId)
            qDebug() << "Failed to get id of File constructor";

        QAndroidJniObject file = m_env->NewObject(fileClass, fileConstrId, sourceDir.object());
        if(!file.isValid())
            qDebug() << "Failed to create File object";

        jmethodID lengthId = m_env->GetMethodID(fileClass, "length", "()J");
        if(!lengthId)
            qDebug() << "Failed to get id of length method";

        jlong dirSize = m_env->CallLongMethod(file.object(), lengthId);
        qDebug() << "App Size:" << static_cast<long>(dirSize/1048576) << "MB";
        /*Getting App Size*/
        /*Getting App Cache Dir Size*/
        jclass contextClass = m_env.findClass("android/content/Context");
        if(!contextClass)
            qDebug() << "Failed to get Context class";

        jmethodID createPackageContextId = m_env->GetMethodID(contextClass, "createPackageContext", "(Ljava/lang/String;I)Landroid/content/Context;");
        if(!createPackageContextId)
            qDebug() << "Failed to get id of createPackageContext method";

        QAndroidJniObject packContext = m_env->CallObjectMethod(context.object(), createPackageContextId, packName, 2);
        if(!packContext.isValid())
            qDebug() << "Failed to createPackageContext";

        /*Cache Dir*/
        jmethodID getCacheDirId = m_env->GetMethodID(contextClass, "getCacheDir", "()Ljava/io/File;");
        if(!getCacheDirId)
            qDebug() << "Failed to get id of getCacheDir method";

        QAndroidJniObject cacheFile = m_env->CallObjectMethod(context.object(), getCacheDirId);
        if(!cacheFile.isValid())
            qDebug() << "Failed to get cache file";

        qDebug() << "Cache dir:" << cacheFile.toString();
        /*Cache Dir*/
        /*External Cache Dir*/
        jmethodID getExternalCacheDirId = m_env->GetMethodID(contextClass, "getExternalCacheDir", "()Ljava/io/File;");
        if(!getExternalCacheDirId)
            qDebug() << "Failed to get id of getExternalCacheDir method";

        QAndroidJniObject externalCacheFile = m_env->CallObjectMethod(context.object(), getExternalCacheDirId);
        if(!externalCacheFile.isValid())
            qDebug() << "Failed to get path of externalChacheFile";

        qDebug() << "External Cache dir:" << externalCacheFile.toString();
        /*External Cache Dir*/
        /*Getting List of Cache Files*/
        jmethodID listFilesId = m_env->GetMethodID(fileClass, "listFiles", "()[Ljava/io/File;");
        if(!listFilesId)
            qDebug() << "Failed to get id of listFiles method";

        QAndroidJniObject fileArray = m_env->CallObjectMethod(cacheFile.object(), listFilesId);
        if(!fileArray.isValid())
            qDebug() << "Failed to get array of cache directory";

        /*Getting List of Cache Files*/
        /*Getting App Cache Dir Size*/
        jlong cacheBytes = 0;
        cacheBytes += sizeOfFiles(fileArray);
        /*Getting App Cache Dir Size*/
        /*Getting App External Cache Dir Size*/
        QAndroidJniObject externalFileArray = m_env->CallObjectMethod(externalCacheFile.object(), listFilesId);
        if(!externalFileArray.isValid())
            qDebug() << "Failed to get list of external cache files";

        cacheBytes += sizeOfFiles(externalFileArray);
        qDebug() << "Size of cache files:" << cacheBytes << " | " << cacheBytes/1024;
        /*Getting App External Cache Dir Size*/
        /* Getting package size */
        /* Getting Instalation/LastUpdate Time */
        jfieldID firstInstTimeId = m_env->GetFieldID(packageInfoClass, "firstInstallTime", "J");
        if(!firstInstTimeId)
            qDebug() << "Failed to get id of firstInstallationTime field";

        jfieldID lastUpdateTimeId = m_env->GetFieldID(packageInfoClass, "lastUpdateTime", "J");
        if(!lastUpdateTimeId)
            qDebug() << "Failed to get id of lastUpdateTime field";

        jlong firstInstallTime = m_env->GetLongField(packageInfo.object(), firstInstTimeId);
        qDebug() << "firstInstTime:" << firstInstallTime;

        jlong lastUpdateTime = m_env->GetLongField(packageInfo.object(), lastUpdateTimeId);
        qDebug() << "lastUpdTime:" << lastUpdateTime;

        jclass dateClass = m_env.findClass("java/util/Date");
        if(!dateClass)
            qDebug() << "Can't find date class";

        jclass simpleDateFormatClass = m_env.findClass("java/text/SimpleDateFormat");
        if(!simpleDateFormatClass)
            qDebug() << "Can't find simpleDateFormat class";

        jclass formatClass = m_env.findClass("java/text/Format");
        if(!formatClass)
            qDebug() << "Can't find format class";

        jmethodID dateInitId = m_env->GetMethodID(dateClass, "<init>", "(J)V");
        if(!dateInitId)
            qDebug() << "Failed to get id of date constructor";

        jmethodID simpleFormatInitId = m_env->GetMethodID(simpleDateFormatClass, "<init>", "(Ljava/lang/String;)V");
        if(!simpleFormatInitId)
            qDebug() << "Failed to get id of simpleFormat constructor";

        jobject installDate = m_env->NewObject(dateClass, dateInitId, firstInstallTime);
        if(!installDate)
            qDebug() << "Failed to create new date file";

        jobject lastUpdateDate = m_env->NewObject(dateClass, dateInitId, lastUpdateTime);
        if(!lastUpdateDate)
            qDebug() << "Failed to create new date file";

        jstring dateFormatStr = m_env->NewStringUTF("yyyy MM dd HH:mm:ss");
        if(!dateFormatStr)
            qDebug() << "Failed to create new string";

        jobject timeFormat = m_env->NewObject(simpleDateFormatClass, simpleFormatInitId, dateFormatStr);
        if(!timeFormat)
            qDebug() << "Failed to create new format object";

        jmethodID formatId = m_env->GetMethodID(formatClass, "format", "(Ljava/lang/Object;)Ljava/lang/String;");
        if(!formatId)
            qDebug() << "Failed to get id of format method id";

        QAndroidJniObject firstInstallDateStr = m_env->CallObjectMethod(timeFormat, formatId, installDate);
        if(!firstInstallDateStr.isValid())
            qDebug() << "Install Date string isn't valid";
        else
            qDebug() << "Install Date Time:" << firstInstallDateStr.toString();

        QAndroidJniObject lastUpdateDateStr = m_env->CallObjectMethod(timeFormat, formatId, lastUpdateDate);
        if(!lastUpdateDateStr.isValid())
            qDebug() << "Install Date string isn't valid";
        else
            qDebug() << "Install Date Time:" << lastUpdateDateStr.toString();
        /* Width */
        jfieldID compatibleWidthLimitDpId = m_env->GetFieldID(appInfoClass, "compatibleWidthLimitDp", "I");
        if(!compatibleWidthLimitDpId)
            qDebug() << "Can't find id of compatibleWidthLimitDp field";

        jfieldID largestWidthLimitDpId = m_env->GetFieldID(appInfoClass, "largestWidthLimitDp", "I");
        if(!largestWidthLimitDpId)
            qDebug() << "Can't find id of largestWidthLimitDp field";

        jfieldID requiresSmallestWidthDpId = m_env->GetFieldID(appInfoClass, "requiresSmallestWidthDp", "I");
        if(!requiresSmallestWidthDpId)
            qDebug() << "Can't find id of requiresSmallestWidthDp field";

        jint compatibleWidthLimitDp = m_env->GetIntField(appInfo.object(), compatibleWidthLimitDpId);
        qDebug() << "Maximum smallest screen width the application is designed for:" << compatibleWidthLimitDp;

        jint largestWidthLimitDp = m_env->GetIntField(appInfo.object(), largestWidthLimitDpId);
        qDebug() << "Maximum smallest screen width the application will work on:" << largestWidthLimitDp;

        jint requiresSmallestWidthDp = m_env->GetIntField(appInfo.object(), requiresSmallestWidthDpId);
        qDebug() << "Required smallest screen width the application can run on:" << requiresSmallestWidthDp;
        /* Width */
        /* taskAffinity */
        jfieldID taskAffinityId = m_env->GetFieldID(appInfoClass, "taskAffinity", "Ljava/lang/String;");
        if(!taskAffinityId)
            qDebug() << "Can't find id of taskAffinity field";

        QAndroidJniObject taskAffinity = m_env->GetObjectField(appInfo.object(), taskAffinityId);
        qDebug() << "Task Affinity:" << taskAffinity.toString();
        /* taskAffinity */
        /* Getting Instalation Time */
    } else {
        qDebug() << "Invalid Index";
    }
}
/*Remove Later*/
