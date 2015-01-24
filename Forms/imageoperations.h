#ifndef IMAGEOPERATIONS_H
#define IMAGEOPERATIONS_H
#include <QObject>
#include <QImage>
#include <QFile>
#include <QDebug>

class imageOperations : public QObject
{
    Q_OBJECT
public:
    explicit imageOperations(QObject *parent = 0);
    ~imageOperations();
    void saveScaledImage(QString,QByteArray,int,int);
signals:

public slots:
};

#endif // IMAGEOPERATIONS_H
