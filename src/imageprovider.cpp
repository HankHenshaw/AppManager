#include "imageprovider.h"

ImageProvider::ImageProvider(QObject *parent)
    : QObject(parent),
      QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage img;
    img = m_mapStrImg.find(id).value();

    if(size)
        *size = img.size();

    if(requestedSize.width() > 0 && requestedSize.height() > 0)
        img = img.scaled(requestedSize.width(), requestedSize.height());

    return img;
}

void ImageProvider::fillMap(QList<QImage> listImg)
{
    for(int i = 0; i < listImg.size(); ++i)
    {
        m_mapStrImg[QString::number(i)] = listImg.at(i);
    }
}
