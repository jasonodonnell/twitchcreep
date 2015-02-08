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

//Clear the networking objectname
void requestHandler::clearObjectName()
{
    networking.setObjectName("");
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

//Takes the stream image request and makes the appropriate network call.
void requestHandler::getStreamImage(QByteArray data, QString name)
{
    db.storeImageFromUsername(name,data);
}

//Get list of all streams without an image in the database
QList<QStringList> requestHandler::getStreamListNoImage()
{
    return db.retrieveStreamListWithoutImage();
}


//Takes the top request and makes the appropriate network call.
void requestHandler::getTop(QByteArray data)
{
    QList<QStringList> topGames;
    db.manageOnlineStreamers("top");
    topGames << jsonParser.getTopGames(data);
    db.storeTopData(topGames);
}

//Takes the top image request and makes the appropriate network call.
void requestHandler::getTopImage(QByteArray data, QString name)
{
    db.storeImageFromTop(name,data);
}

//Takes the usrername request and makes the appropriate network call.
void requestHandler::getUsername(QByteArray data, QString name)
{
    bool exists;
    exists = jsonParser.checkUsernameExists(data);
    if(exists)
    {
        settings.setValue("username",name);
        db.truncateStreamData();
        db.truncateTopData();
    }
    else
    {
        db.truncateStreamData();
        db.truncateTopData();
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
    networking.makeFollowRequest(username);
}

//Get image for a given stream
void requestHandler::makeImageRequest(QStringList stream)
{
    networking.makeStreamImageRequest(stream);
}

//Make network request from a search value
void requestHandler::makeSearchRequest(QString search)
{
    networking.makeGameRequest(search);
}

//Make top request
void requestHandler::makeTopRequest()
{
    networking.makeTopGamesRequest();
}

//Read stream image from the database
QByteArray requestHandler::readStreamImage(QString name)
{
    return db.retrieveStreamImage(name);
}

//Read top image from the database
QByteArray requestHandler::readTopImage(QString name)
{
    return db.retrieveTopImage(name);
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
    else if (jsonType == "top")
        this->getTop(data);
    else if (jsonType.contains("streamImage:"))
    {
        QStringList name = jsonType.split(":");
        this->getStreamImage(data,name[1]);
    }
    else if (jsonType.contains("topImage:"))
    {
        QStringList name = jsonType.split(":");
        this->getTopImage(data,name[1]);
    }
    else if (jsonType.contains("usernameCheck:"))
    {
        QStringList name = jsonType.split(":");
        this->getUsername(data,name[1]);
    }
    else if (jsonType.contains("game"))
        this->getGame(data);

    if(networking.objectName() != "followsList")
        networking.setObjectName("");
}

//Read the database on a timed interval.  Triggered by the timers class.
QList<QStringList> requestHandler::timedDatabaseRead(int index)
{
    QList<QStringList> streamDataList;
    if(index == 0)
        streamDataList = db.retreiveStreamList("featured");
    else if(index == 1)
        streamDataList = db.retrieveTopList();
    else if(index == 2)
        streamDataList = db.retreiveStreamList("follow");
    else if(index == 3)
        streamDataList = db.retreiveStreamList("search");
    return streamDataList;
}
