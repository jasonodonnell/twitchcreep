#include "json.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QStringList>
#include <QDebug>

json::json(QObject *parent) : QObject(parent)
{
}

json::~json()
{

}

QStringList json::getStreamerFollowedList(QByteArray data)
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

QList<QStringList> json::getFeaturedStreamData(QByteArray data)
{
    QList<QStringList> streamerList;
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["featured"].toArray();
    if (array.isEmpty() != true)
        for(int i = 0; i != array.count(); ++i)
        {
            QStringList streamer;
            QJsonObject featured = array[i].toObject();
            QJsonValue streamVal = featured.value("stream");
            QJsonObject streamObj = streamVal.toObject();

            QJsonValue channelVal = streamObj.value("channel");
            QJsonObject channelObj = channelVal.toObject();

            QString game = streamObj.value("game").toString();
            QString viewers = QString::number(streamObj.value("viewers").toInt());
            QString status = channelObj.value("status").toString();
            QString displayName = channelObj.value("display_name").toString();
            QString logo = channelObj.value("logo").toString();
            QString url = channelObj.value("url").toString();
            streamer << game << viewers << status << displayName << logo << url;
            streamerList << streamer;
    }
    return streamerList;
}

QStringList json::getStreamData(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonValue streamVal = json.value("stream");
    if (streamVal.isNull())
    {
        QStringList streamNull;
        return streamNull;
    }
    else
    {
        QJsonObject streamObj = streamVal.toObject();

        QJsonValue channelVal = streamObj.value("channel");
        QJsonObject channelObj = channelVal.toObject();

        QString game = streamObj.value("game").toString();
        QString viewers = QString::number(streamObj.value("viewers").toInt());
        QString status = channelObj.value("status").toString();
        QString displayName = channelObj.value("display_name").toString();
        QString logo = channelObj.value("logo").toString();
        QString url = channelObj.value("url").toString();
        QStringList streamer;
        streamer << game << viewers << status << displayName << logo << url;
        return streamer;
    }
}

QList<QStringList> json::getTopGames(QByteArray data)
{
    QList<QStringList> topList;

    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["top"].toArray();

    if (array.isEmpty() != true)
        for(int i = 0; i != array.count(); ++i)
        {
            QStringList topGames;
            QJsonObject topObj = array[i].toObject();
            QString viewers = QString::number(topObj.value("viewers").toInt());

            QJsonValue gameVal = topObj.value("game");
            QJsonObject gameObj = gameVal.toObject();

            QString game = gameObj.value("name").toString();

            QJsonValue logoVal = gameObj.value("logo");
            QJsonObject logoObj = logoVal.toObject();

            QString smallLogo = logoObj.value("small").toString();

            topGames << game << viewers << smallLogo;
            topList << topGames;
        }
    return topList;
}
