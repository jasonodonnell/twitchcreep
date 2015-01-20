#include "imageoperations.h"
#include <QImage>
#include <QFile>
#include <QDebug>

imageOperations::imageOperations(QObject *parent) : QObject(parent)
{

}

imageOperations::~imageOperations()
{

}

void imageOperations::saveScaledImage(QString filename, QByteArray data, int width, int height)
{
    QImage image = QImage::fromData(data);
    QImage imageScaled = image.scaled(width,height);
    QFile imageFile(filename);
    if (imageFile.open(QIODevice::WriteOnly))
    {
      imageScaled.save(&imageFile, "PNG");
      imageFile.close();
      qDebug() << "Image saved to" << filename;
    }
}
