#include "timers.h"

timers::timers(QObject *parent) : QObject(parent)
{
    this->createTimerSignals();
    this->startTimers();
}

timers::~timers()
{

}

void timers::createTimerSignals()
{
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(dataRequest()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(databaseRead()));
}

void timers::startTimers()
{
    requestTimer->start(5000);
    readTimer->start(500);
}

void timers::dataRequest()
{
    emit(requestData());
}

void timers::databaseRead()
{
    emit(readDatabase());
}
