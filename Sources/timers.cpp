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
    connect(connectionTimer,SIGNAL(timeout()),this,SLOT(networkConnection()));
    connect(displayedOfflineTimer,SIGNAL(timeout()),this,SLOT(displayedOffline()));
    connect(networkRequestTimer,SIGNAL(timeout()),this,SLOT(makeNetworkRequest()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(databaseRead()));
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(dataRequest()));
    connect(updateStreamsTimer,SIGNAL(timeout()),this,SLOT(updateDisplayedStreams()));
}

void timers::startTimers()
{
    requestTimer->start(2000);
    readTimer->start(50);
    connectionTimer->start(5000);
    displayedOfflineTimer->start(10000);
    networkRequestTimer->start(100);
    updateStreamsTimer->start(60000);
}

void timers::databaseRead()
{
    emit(readDatabase());
}

void timers::dataRequest()
{
    emit(requestData());
}

void timers::displayedOffline()
{
    emit(removeOfflineStreams());
}

void timers::makeNetworkRequest()
{
    emit(networkRequest());
}

void timers::networkConnection()
{
    emit(checkConnection());
}

void timers::updateDisplayedStreams()
{
    emit(updateStreams());
}
