#include "timers.h"

timers::timers(QObject *parent) : QObject(parent)
{
    this->createTimerSignals();
    this->dataRequest();
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
    connect(displayedOfflineTimer,SIGNAL(timeout()),this,SLOT(displayedOffline()));
    connect(networkRequestTimer,SIGNAL(timeout()),this,SLOT(makeNetworkRequest()));
}

void timers::startTimers()
{
    requestTimer->start(3000);
    readTimer->start(50);
    connectionTimer->start(5000);
    displayedOfflineTimer->start(10000);
    networkRequestTimer->start(100);
}

void timers::databaseRead()
{
    emit(readDatabase());
}

void timers::dataRequest()
{
    emit(requestData());
}

void timers::makeNetworkRequest()
{
    emit(networkRequest());
}

void timers::networkConnection()
{
    emit(checkConnection());
}

void timers::displayedOffline()
{
    emit(removeOfflineStreams());
}
