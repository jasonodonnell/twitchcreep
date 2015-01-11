#include "json.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>

json::json(QObject *parent) : QObject(parent)
{
    QString url = "https://api.twitch.tv/kraken/users/L0veWizard/follows/channels";
    this->makeRequest(url);
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timedFollowRequest()));
    timer->start(60000);
}

json::~json()
{

}

void json::makeRequest(QString url)
{
    networkManager->get(QNetworkRequest(QUrl(url)));
}


QStringList json::getStreamersList(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["follows"].toArray();
    QStringList streamers;
    for(int i = 0; i != array.count(); ++i)
    {
        QJsonObject follows = array[i].toObject();
        QJsonValue streamersVal =  follows.value("_links");
        QJsonObject streamerObj = streamersVal.toObject();
        QString streamer = streamerObj.value("self").toString();
        QStringList url = streamer.split("/");
        streamers << url.value(url.length() - 1);
    }
    return streamers;
}

void json::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    emit(dataReady(replyData));
}

void json::timedFollowRequest()
{
    QString url = "https://api.twitch.tv/kraken/users/L0veWizard/follows/channels";
    this->makeRequest(url);
}
