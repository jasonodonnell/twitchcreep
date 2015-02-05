#include "networkoperations.h"

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    //Signal-slot connection for network reply, when its finished, activates doneReading (passess the reply)
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
}

//Makes the network request to twitch api
void networkOperations::makeRequest(QString url)
{
    if(this->checkNetworkConnection())
        networkManager->get(QNetworkRequest(QUrl(url)));
}

//Slot that is activated after finished signal is fired.  Reads
//data into a byte array, sets the objectname and emits a signal.
void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    QString requestType = this->objectName();
    emit(dataReady(replyData,requestType));
}

//Make the follow request based on username in QSettings
void networkOperations::makeFollowRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
    this->setObjectName("follows");
    this->makeRequest(url);
}

//Make the stream request based on the username passed in
void networkOperations::makeStreamRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/streams/" + username;
    if(this->objectName().isEmpty())
    {
        this->setObjectName("stream");
        this->makeRequest(url);
    }
}

//Makes the stream requests from a list of streamer usernames.
void networkOperations::makeStreamRequestFromList(QStringList usernames)
{
    foreach(QString username,usernames)
    {
        QString url = "https://api.twitch.tv/kraken/streams/" + username;
        this->setObjectName("followsList");
        this->makeRequest(url);
    }
}

//Makes a featured stream request.
void networkOperations::makeFeaturedRequest()
{
    QString url = "https://api.twitch.tv/kraken/streams/featured";
    if(this->objectName().isEmpty())
    {
        this->setObjectName("featured");
        this->makeRequest(url);
    }
}

//Makes a top games request.
void networkOperations::makeTopGamesRequest()
{
    QString url = "https://api.twitch.tv/kraken/games/top";
    if(this->objectName().isEmpty())
    {
        this->setObjectName("top");
        this->makeRequest(url);
    }
}

//Gets the profile image for a stream
void networkOperations::makeStreamImageRequest(QStringList streamDataList)
{
    QString objectName = "streamImage:" + streamDataList[0];
    QString url = streamDataList[1];
    if(url.isEmpty())
        url = "http://static-cdn.jtvnw.net/jtv-static/404_preview-300x300.png";
    if(this->objectName().isEmpty())
    {
        this->setObjectName(objectName);
        this->makeRequest(url);
    }
}

//Gets the profile image for a game
void networkOperations::makeTopImageRequest(QStringList topDataList)
{
    QString objectName = "topImage:" + topDataList[0];
    QString url = topDataList[1];
    if(this->objectName().isEmpty())
    {
        this->setObjectName(objectName);
        this->makeRequest(url);
    }
}

//Check if username exists.
void networkOperations::checkUsernameRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username;
    QString objectName = "usernameCheck:" + username;
    if(this->objectName().isEmpty())
    {
        this->setObjectName(objectName);
        this->makeRequest(url);
    }
}

//Gets list of streams for a game
void networkOperations::makeGameRequest(QString game)
{
    QString url = "https://api.twitch.tv/kraken/search/streams?q=" + game;
    if(this->objectName().isEmpty())
    {
        this->setObjectName("game");
        this->makeRequest(url);
    }
}

//Check current internet connection
bool networkOperations::checkNetworkConnection()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

networkOperations::~networkOperations()
{

}

