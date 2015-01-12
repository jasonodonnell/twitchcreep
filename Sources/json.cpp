#include "json.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QObject>
#include <QStringList>

json::json(QObject *parent) : QObject(parent)
{
}

json::~json()
{

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
