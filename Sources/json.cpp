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

//There should definitely be a better way of doing this.  I'm not
//familiar enough with the framework yet to know.  QSignalMapper
//looks promising to bind each signal to the object.
//For now I will just parse the data and determine what kind it is.
void json::determineDataSource(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QStringList keys = json.keys();

    if (keys.contains("stream"))
    {
        qDebug() << "Stream button";
    }
    else if (keys.contains("featured"))
    {
        qDebug() << "Featured button";
    }
    else if (keys.contains("top"))
    {
        qDebug() << "Top button";
    }
    else if (keys.contains("follows"))
    {
        qDebug() << "Follows button";
    }
}

QStringList json::getStreamerList(QByteArray data)
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
