#include "../Headers/timers.h"

timers::timers(QObject *parent) : QObject(parent)
{
    this->createTimerSignals();
    this->dataRequest();
    this->makeNetworkRequest();
    this->networkConnection();
    this->startTimers();

}

timers::~timers()
{

}
//Attach all the signals to slots.
void timers::createTimerSignals()
{
    connect(connectionTimer,SIGNAL(timeout()),this,SLOT(networkConnection()));
    connect(networkRequestTimer,SIGNAL(timeout()),this,SLOT(makeNetworkRequest()));
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(dataRequest()));
}

//Start all the timers.
void timers::startTimers()
{
    requestTimer->start(3000);
    connectionTimer->start(5000);
    networkRequestTimer->start(150);
}

//Request data on a timer.
void timers::dataRequest()
{
    emit(requestData());
}

//Make a network request based on the current open tab.
void timers::makeNetworkRequest()
{
    emit(networkRequest());
}

//Check network connection on a timer (for status bar)
void timers::networkConnection()
{
    emit(checkConnection());
}
