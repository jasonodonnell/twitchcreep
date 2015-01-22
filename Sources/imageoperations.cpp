#include "imageoperations.h";

imageOperations::imageOperations(QObject *parent) : QObject(parent)
{

}

imageOperations::~imageOperations()
{

}

//This receives a QByteArray that is an image, the image is resized to 30x30
//then saved with username as the filename.
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
