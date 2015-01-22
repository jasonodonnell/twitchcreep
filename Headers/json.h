#ifndef JSONRETRIEVER_H
#define JSONRETRIEVER_H
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QDebug>

class json : public QObject
{
    Q_OBJECT
public:
    explicit json(QObject *parent = 0);
    QStringList getStreamerFollowedList(QByteArray);
    ~json();
    QStringList getStreamData(QByteArray);
    QList<QStringList> getFeaturedStreamData(QByteArray);
    QList<QStringList> getTopGames(QByteArray);
signals:

public slots:

private:
};

#endif // JSON_H
