#include "networkoperations.h"
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    QString username = "L0veWizard";
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timedFollowRequest()));
    timer->start(60000);

}

void networkOperations::makeRequest(QString url)
{
    int connection = this->checkNetworkConnection();
    qDebug() << connection;
    if (connection == 0)
        networkManager->get(QNetworkRequest(QUrl(url)));
    else
        qDebug() << "Not connected to internet";
}

void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    emit(dataReady(replyData));
}

void networkOperations::timedFollowRequest()
{
    QString username = "L0veWizard";
    QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
    this->makeRequest(url);

}

void networkOperations::makeFollowRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels";
    this->makeRequest(url);
}

void networkOperations::makeStreamRequest(QString username)
{
    QString url = "https://api.twitch.tv/kraken/streams/" + username;
    this->makeRequest(url);
}

void networkOperations::makeFeaturedRequest()
{
    QString url = "https://api.twitch.tv/kraken/streams/featured";
    this->makeRequest(url);
}

void networkOperations::makeTopGamesRequest()
{
    QString url = "https://api.twitch.tv/kraken/games/top";
    this->makeRequest(url);
}


int networkOperations::checkNetworkConnection()
{
    QNetworkConfigurationManager mgr;
    if(mgr.isOnline())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

networkOperations::~networkOperations()
{

}

