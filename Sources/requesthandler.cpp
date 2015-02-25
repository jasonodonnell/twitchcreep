#include "requesthandler.h"

requestHandler::requestHandler(QObject *parent) : QThread(parent)
{
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestProcess(QByteArray,QString)));
}

requestHandler::~requestHandler()
{

}

//Takes the featured request and makes the appropriate network call.
void requestHandler::changeDisplayVariable(QString requestType, QString username)
{
    db.manageDisplayVariable(requestType, username);
}

//Check internet connection
bool requestHandler::checkConnection()
{
    return networking.checkNetworkConnection();
}

//Check if username exists, used in the UI
void requestHandler::checkUsername(QString text)
{
    networking.checkUsernameRequest(text);
}

//Takes the featured request and makes the appropriate network call.
void requestHandler::getFeatured(QByteArray data)
{
    QList<QStringList> streamerList;
    QStringList streamData;
    db.manageOnlineStreamers("featured");
    streamerList << jsonParser.getFeaturedStreamData(data);

    foreach(streamData, streamerList)
        db.storeStreamData(streamData, "featured");
}

//Takes the follows request and makes the appropriate network call.
void requestHandler::getFollows(QByteArray data)
{
    QStringList follows;
    db.manageOnlineStreamers("followed");
    follows = jsonParser.getStreamerFollowedList(data);
    networking.makeStreamRequestFromList(follows);
}

//Takes the follow list and makes the appropriate network calls.
void requestHandler::getFollowsList(QByteArray data)
{
    QStringList streamData = jsonParser.getStreamData(data);
    if(!streamData.isEmpty())
        db.storeStreamData(streamData, "followed");
}

//Get games from the search query
void requestHandler::getGame(QByteArray data)
{
    QList<QStringList> search = jsonParser.getGameStreamData(data);
    QStringList searchData;
    db.manageOnlineStreamers("search");
    foreach(searchData, search)
        db.storeStreamData(searchData, "search");
}

//Returns status from stream for tooltip
QString requestHandler::getStatus(QString username)
{
    return db.retrieveStatus(username);
}

//Takes the usrername request and makes the appropriate network call.
void requestHandler::getUsername(QByteArray data, QString name)
{
    QString settingsDir = QCoreApplication::applicationDirPath() + "/twitchCreep.conf";
    QSettings settings(settingsDir, QSettings::IniFormat);
    bool exists;
    exists = jsonParser.checkUsernameExists(data);
    if(exists)
    {
        settings.setValue("username",name);
        db.truncateStreamData();
    }
    else
    {
        db.truncateStreamData();
        settings.setValue("username","");
        emit(usernameDialogSignal("error"));
    }
    emit(clearFollowList());
}

//Request featured streams
void requestHandler::makeFeaturedRequest()
{
    networking.makeFeaturedRequest();
}

//Make follow request
void requestHandler::makeFollowRequest(QString username)
{
    if(!username.isEmpty())
        networking.makeFollowRequest(username);
}

//Makes a network request
void requestHandler::makeRequest()
{
    networking.makeRequest();
}

//Make network request from a search value
void requestHandler::makeSearchRequest(QString search)
{
    networking.makeGameRequest(search);
}

//Slot where data requests are processed.  When a request is made, the object
//name is set by the member function and passed into this along with the request
//data.  From here it is directed to the appropriate member function.
void requestHandler::requestProcess(QByteArray data, QString jsonType)
{
    if (jsonType == "follows")
        this->getFollows(data);
    else if (jsonType == "followsList")
        this->getFollowsList(data);
    else if (jsonType == "featured")
        this->getFeatured(data);
    else if (jsonType.contains("usernameCheck:"))
    {
        QStringList name = jsonType.split(":");
        this->getUsername(data,name[1]);
    }
    else if (jsonType.contains("game"))
        this->getGame(data);

    networking.popRequestFromList();
}

//Read the database on a timed interval.  Triggered by the timers class.
QList<QStringList> requestHandler::timedDatabaseRead(int index)
{
    QList<QStringList> streamDataList;
    if(index == 0)
        streamDataList = db.retreiveStreamList("featured");
    else if(index == 1)
        streamDataList = db.retreiveStreamList("follow");
    else if(index == 2)
        streamDataList = db.retreiveStreamList("search");
    return streamDataList;
}

//Read the database on a timed interval.  Triggered by the timers class.
QList<QStringList> requestHandler::timedDatabaseUpdateRead(int index)
{
    QList<QStringList> streamDataList;
    if(index == 0)
    {
        db.manageDisplayVariableClear("featured");
        streamDataList = this->timedDatabaseRead(index);
    }
    else if(index == 1)
    {
        db.manageDisplayVariableClear("follow");
        streamDataList = this->timedDatabaseRead(index);
    }
    else if(index == 2)
    {
        db.manageDisplayVariableClear("search");
        streamDataList = this->timedDatabaseRead(index);
    }
    return streamDataList;
}

//Remove streams on a timer.
QStringList requestHandler::timedOfflineRemoval(int index)
{
    QStringList streamDataList;
    if(index == 0)
        streamDataList = db.getDisplayedOfflineStreams("featured");
    else if(index == 1)
        streamDataList = db.getDisplayedOfflineStreams("followed");
    else if(index == 2)
        streamDataList = db.getDisplayedOfflineStreams("search");
    return streamDataList;
}
