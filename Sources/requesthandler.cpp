#include "requesthandler.h"

requestHandler::requestHandler(QObject *parent) : QObject(parent)
{
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestProcess(QByteArray,QString)));
}

requestHandler::~requestHandler()
{

}

void requestHandler::requestProcess(QByteArray data, QString jsonType)
{
    if (jsonType == "follows")
        this->getFollows(data);
    else if (jsonType == "followList")
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
}

void requestHandler::getFollows(QByteArray data)
{
    QStringList follows;
    follows = jsonParser.getStreamerFollowedList(data);
    networking.makeStreamRequestFromList(follows);
}

void requestHandler::getFollowsList(QByteArray data)
{
    QStringList streamData = jsonParser.getStreamData(data);
    if(!streamData.isEmpty())
        db.storeStreamData(streamData, "followed");
}

void requestHandler::getFeatured(QByteArray data)
{
    QList<QStringList> streamerList;
    QStringList streamData;
    streamerList << jsonParser.getFeaturedStreamData(data);
    foreach(streamData, streamerList)
        db.storeStreamData(streamData, "featured");
        networking.makeStreamImageRequest(streamData);
}

void requestHandler::getTop(QByteArray data)
{
    QList<QStringList> topGames;
    topGames << jsonParser.getTopGames(data);
    db.storeTopData(topGames);
}

void requestHandler::getStreamImage(QByteArray data, QString name)
{
    db.storeImageFromUsername(name,data);
}

void requestHandler::getTopImage(QByteArray data, QString name)
{
    db.storeImageFromTop(name,data);
}

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

void requestHandler::getGame(QByteArray data)
{
    db.truncateStreamData();
    QList<QStringList> search = jsonParser.getGameStreamData(data);
    QStringList searchData;
    foreach(searchData, search)
        db.storeStreamData(searchData, "search");
        networking.makeStreamImageRequest(searchData);
}

void requestHandler::checkUsername(QString text)
{
    networking.checkUsernameRequest(text);
}

void requestHandler::makeFeaturedRequest()
{
    networking.makeFeaturedRequest();
}

void requestHandler::makeTopRequest()
{
    networking.makeTopGamesRequest();
}

void requestHandler::makeFollowRequest(QString username)
{
    networking.makeFollowRequest(username);
}

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

void requestHandler::makeSearchRequest(QString search)
{
    networking.makeGameRequest(search);
}

bool requestHandler::checkConnection()
{
    return networking.checkNetworkConnection();
}
