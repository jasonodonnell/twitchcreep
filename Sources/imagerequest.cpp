#include "imagerequest.h"

imageRequest::imageRequest(QObject *parent) : QThread(parent)
{

}

void imageRequest::run(QList<QStringList> streamsNoImage )
{
    QStringList streamData;
    foreach(streamData, streamsNoImage)
    {
        emit(requestImageSignal(streamData));
        this->msleep(100);
    }
}

imageRequest::~imageRequest()
{

}

