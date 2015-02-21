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

//Start all the timers.
void timers::startTimers()
{
    requestTimer->start(2000);
    readTimer->start(50);
    connectionTimer->start(5000);
    displayedOfflineTimer->start(10000);
    networkRequestTimer->start(100);
    updateStreamsTimer->start(60000);
}

//Read the database for updates on a current tab index.  Will probably go away
//when the architecture is changed.
void timers::databaseRead()
{
    emit(readDatabase());
}

//Request data on a timer.
void timers::dataRequest()
{
    emit(requestData());
}

//Timed offline stream removals.
void timers::displayedOffline()
{
    emit(removeOfflineStreams());
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

//Update streams in listviews for changes.
void timers::updateDisplayedStreams()
{
    emit(updateStreams());
}
