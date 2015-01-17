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
QString json::determineDataSource(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QStringList keys = json.keys();
    //Last value in the array is the type of data.
    int last = keys.length()-1;
    return keys[last];
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

void json::getFeaturedStreams(QByteArray data)
{
    QJsonDocument jsonData = QJsonDocument::fromJson(data);
    QJsonObject json = jsonData.object();
    QJsonArray array = json["featured"].toArray();
    for(int i = 0; i != array.count(); ++i)
    {
        QJsonObject featured = array[i].toObject();
        QJsonValue streamVal = featured.value("stream");
        QJsonObject streamObj = streamVal.toObject();

        QJsonValue channelVal = streamObj.value("channel");
        QJsonObject channelObj = channelVal.toObject();

        QString game = streamObj.value("game").toString();
        int viewers = streamObj.value("viewers").toInt();
        QString status = channelObj.value("status").toString();
        QString displayName = channelObj.value("display_name").toString();
        QString logo = channelObj.value("logo").toString();
        QString url = channelObj.value("url").toString();
        qDebug() << "###################################";
        qDebug() << "User:" << displayName << endl <<
                    "Game:" << game << endl <<
                    "Status:" << status << endl <<
                    "Viewers:" << viewers;
    }
}

