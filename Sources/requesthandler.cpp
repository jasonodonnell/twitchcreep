#include "../Headers/requesthandler.h"

requestHandler::requestHandler(QObject *parent) : QObject(parent)
{
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestProcess(QByteArray,QString)));
    QString settingsDir = QCoreApplication::applicationDirPath() + "/twitchCreep.conf";
    settings.setPath(QSettings::NativeFormat,QSettings::UserScope,settingsDir);
}

requestHandler::~requestHandler()
{

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
    emit(manageOnlineStreamers("featured"));
    streamerList << jsonParser.getFeaturedStreamData(data);
    foreach(streamData, streamerList)
    {
        emit(storeStreamData(streamData, "featured"));
    }
}

//Takes the follows request and makes the appropriate network call.
void requestHandler::getFollows(QByteArray data)
{
    QStringList follows;
    emit(manageOnlineStreamers("followed"));
    follows = jsonParser.getStreamerFollowedList(data);
    networking.makeStreamRequestFromList(follows);
}

//Takes the follow list and makes the appropriate network calls.
void requestHandler::getFollowsList(QByteArray data)
{
    QStringList streamData = jsonParser.getStreamData(data);
    if(!streamData.isEmpty())
        emit(storeStreamData(streamData, "followed"));
}

//Get games from the search query
void requestHandler::getGame(QByteArray data)
{
    QList<QStringList> search = jsonParser.getGameStreamData(data);
    QStringList searchData;
    emit(manageOnlineStreamers("search"));
    foreach(searchData, search)
        emit(storeStreamData(searchData, "search"));
}

//Retrieve value of a given setting
QString requestHandler::getSettingsValue(QString value)
{
    return settings.value(value).toString();
}

//Takes the usrername request and makes the appropriate network call.
void requestHandler::getUsername(QByteArray data, QString name)
{
    bool exists;
    exists = jsonParser.checkUsernameExists(data);
    if(exists)
    {
        settings.setValue("username",name);
        emit(truncateStreamData());
    }
    else
    {
        emit(truncateStreamData());
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
    {
        emit(truncateSearchData());
        this->getGame(data);
    }
    //networking.popRequestFromList();
}

//Set the value of a given setting
void requestHandler::setSettingsValue(QString setting, QString value)
{
    settings.setValue(setting,value);
}

//Store the index of an item from the view
void requestHandler::storeItemIndex(QString requestType, QString username, int index)
{
    emit(storeIndex(requestType,username,index));
}
