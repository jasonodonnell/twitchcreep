#include "networkoperations.h"
#include <QNetworkAccessManager>

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    QString url = "https://api.twitch.tv/kraken/users/L0veWizard/follows/channels";
    this->makeRequest(url);
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timedFollowRequest()));
    timer->start(60000);
}

void networkOperations::makeRequest(QString url)
{
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    emit(dataReady(replyData));
}

void networkOperations::timedFollowRequest()
{
    QString url = "https://api.twitch.tv/kraken/users/L0veWizard/follows/channels";
    this->makeRequest(url);
}

networkOperations::~networkOperations()
{

}

