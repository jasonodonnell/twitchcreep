#include "networkoperations.h"

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    //Signal-slot connection for network reply, when its finished, activates doneReading (passess the reply)
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
}

networkOperations::~networkOperations()
{

}

void networkOperations::addRequestToList(QString requestType, QString url)
{
    QStringList requestList;
    requestList << requestType << url;
    requests << requestList;
}

//Check current internet connection
bool networkOperations::checkNetworkConnection()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

//Check if username exists.
void networkOperations::checkUsernameRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username;
    QString objectName = "usernameCheck:" + username;
    this->addRequestToList(objectName,url);
}

//Slot that is activated after finished signal is fired.  Reads
//data into a byte array, sets the objectname and emits a signal.
void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    QString requestType = this->objectName();
    emit(dataReady(replyData,requestType));
}

//Makes a featured stream request.
void networkOperations::makeFeaturedRequest()
{
    QString url = "https://api.twitch.tv/kraken/streams/featured";
    this->addRequestToList("featured",url);
}

//Make the follow request based on username in QSettings
void networkOperations::makeFollowRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
    this->addRequestToList("follows",url);
}

//Gets list of streams for a game
void networkOperations::makeGameRequest(QString game)
{
    QString url = "https://api.twitch.tv/kraken/search/streams?q=" + game;
    this->addRequestToList("game",url);
}

//Makes the network request to twitch api
void networkOperations::makeRequest()
{
    if(requests.count() > 0)
    {
        if(this->checkNetworkConnection())
        {
            QStringList requestInfo = requests.first();
            this->setObjectName(requestInfo[0]);
            networkManager->get(QNetworkRequest(QUrl(requestInfo[1])));
        }
    }
}


//Make the stream request based on the username passed in
void networkOperations::makeStreamRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/streams/" + username;
    this->addRequestToList("stream",url);
}

//Makes the stream requests from a list of streamer usernames.
void networkOperations::makeStreamRequestFromList(QStringList usernames)
{
    foreach(QString username,usernames)
    {
        QString url = "https://api.twitch.tv/kraken/streams/" + username;
        this->addRequestToList("followsList",url);
    }
}

//Gets the profile image for a stream
void networkOperations::makeStreamImageRequest(QStringList streamDataList)
{
    QStringList checkList;
    QString objectName = "streamImage:" + streamDataList[0];
    QString url = streamDataList[1];
    if(url.isEmpty())
        url = "http://static-cdn.jtvnw.net/jtv-static/404_preview-300x300.png";

    checkList << objectName << url;
    if(!requests.contains(checkList))
        this->addRequestToList(objectName,url);
}

void networkOperations::popRequestFromList()
{
    if(requests.count() > 0)
        requests.pop_front();
}
