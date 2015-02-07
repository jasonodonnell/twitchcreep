#include "timers.h"

timers::timers(QObject *parent) : QObject(parent)
{
    this->createTimerSignals();
    this->dataRequest();
    this->makeImageRequest();
    this->startTimers();
}

timers::~timers()
{

}

void timers::createTimerSignals()
{
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(dataRequest()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(databaseRead()));
    connect(connectionTimer,SIGNAL(timeout()),this,SLOT(networkConnection()));
    connect(imageRequestTimer,SIGNAL(timeout()),this,SLOT(makeImageRequest()));
}

void timers::startTimers()
{
    requestTimer->start(5000);
    readTimer->start(100);
    connectionTimer->start(5000);
    imageRequestTimer->start(10);
}

void timers::databaseRead()
{
    emit(readDatabase());
}

void timers::dataRequest()
{
    emit(requestData());
}

void timers::makeImageRequest()
{
    emit(imageRequest());
}

void timers::networkConnection()
{
    emit(checkConnection());
}
