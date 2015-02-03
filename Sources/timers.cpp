#include "timers.h"

timers::timers(QObject *parent) : QObject(parent)
{
    this->createTimerSignals();
    this->dataRequest();
    this->databaseRead();
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
}

void timers::startTimers()
{
    requestTimer->start(5000);
    readTimer->start(500);
    connectionTimer->start(5000);
}

void timers::dataRequest()
{
    emit(requestData());
}

void timers::databaseRead()
{
    emit(readDatabase());
}

void timers::networkConnection()
{
    emit(checkConnection());
}
