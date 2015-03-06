#include "Headers/json.h"

json::json(QObject *parent) : QObject(parent)
{

}

json::~json()
{

}

bool json::checkUsernameExists(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    if(json.contains("error"))
        return false;
    else
        return true;
}

//Returns a QList<QStringList> of all the featured streams.
//The json object is very nested so some unpacking is required.
QList<QStringList> json::getFeaturedStreamData(QByteArray data)
{
    QList<QStringList> streamerList;
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["featured"].toArray();
    if (!array.isEmpty())
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
            streamer << displayName << game << viewers << status << logo << url;
            streamerList << streamer;
    }
    return streamerList;
}

QList<QStringList> json::getGameStreamData(QByteArray data)
{
    QList<QStringList> streamerList;
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["streams"].toArray();

    if (!array.isEmpty())
    {
        for(int i = 0; i != array.count(); ++i)
        {
            QStringList streamer;
            QJsonObject gameObj = array[i].toObject();
            QString viewers = QString::number(gameObj.value("viewers").toInt());

            QJsonValue channelVal = gameObj.value("channel");
            QJsonObject channelObj = channelVal.toObject();

            QString game = channelObj.value("game").toString();
            QString status = channelObj.value("status").toString();
            QString displayName = channelObj.value("display_name").toString();
            QString logo = channelObj.value("logo").toString();
            QString url = channelObj.value("url").toString();
            streamer << displayName << game << viewers << status << logo << url;
            streamerList << streamer;
        }
    }
    else
    {
        QStringList streamerNotFound;
        streamerNotFound << "Not found" << "" << "" << "" << "" << "";
        streamerList << streamerNotFound;
    }
    return streamerList;
}
//Returns a QStringList of a stream.
//The json object is very nested so some unpacking is required.
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
        streamer << displayName << game << viewers << status << logo << url;
        return streamer;
    }
}

//Returns a QStringList of all the streamers followed by a user.
//The json object is very nested so some unpacking is required.
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

