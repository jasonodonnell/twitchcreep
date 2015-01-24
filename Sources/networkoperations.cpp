#include "networkoperations.h"

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    //Signal-slot connection for network reply, when its finished, activates doneReading (passess the reply)
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
    //Timer thread that makes a request every 60 seconds in the background.
    //Will have 5 second threads for active tab windows.
    connect(timer,SIGNAL(timeout()),this,SLOT(timedFollowRequest()));
    timer->start(60000);
}

//Makes the network request to twitch api
void networkOperations::makeRequest(QString url)
{
    if(this->checkNetworkConnection())
        networkManager->get(QNetworkRequest(QUrl(url)));
    else
        qDebug() << "Not connected to internet";
}

//Slot that is activated after finished signal is fired.  Reads
//data into a byte array, sets the objectname and emits a signal.
void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    QString requestType = this->objectName();
    emit(dataReady(replyData,requestType));
}

void networkOperations::timedFollowRequest()
{
    QString username = "L0veWizard";
    if (!username.isNull())
    {
        QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
        this->setObjectName("follow");
        this->makeRequest(url);
    }
}

void networkOperations::makeFollowRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
    this->setObjectName("follows");
    this->makeRequest(url);
}

void networkOperations::makeStreamRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/streams/" + username;
    this->setObjectName("stream");
    this->makeRequest(url);
}

void networkOperations::makeStreamRequestFromList(QStringList usernames)
{
    foreach(QString username,usernames)
    {
        QString url = "https://api.twitch.tv/kraken/streams/" + username;
        this->setObjectName("followList");
        this->makeRequest(url);
    }
}


void networkOperations::makeFeaturedRequest()
{
    QString url = "https://api.twitch.tv/kraken/streams/featured";
    this->setObjectName("featured");
    this->makeRequest(url);
}

void networkOperations::makeTopGamesRequest()
{
    QString url = "https://api.twitch.tv/kraken/games/top";
    this->setObjectName("top");
    this->makeRequest(url);
}

void networkOperations::makeImageRequest(QString url,QString username)
{
    QString imageFilename = "image:" + username;
    this->setObjectName(imageFilename);
    this->makeRequest(url);
}


bool networkOperations::checkNetworkConnection()
{
    QNetworkConfigurationManager mgr;
    if(mgr.isOnline())
        return true;
    else
        return false;
}

networkOperations::~networkOperations()
{

}

