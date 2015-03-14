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
    ~json();
    bool checkUsernameExists(QByteArray data);
    QList<QStringList> getFeaturedStreamData(QByteArray);
    QList<QStringList> getGameStreamData(QByteArray data);
    QStringList getStreamData(QByteArray);
    QStringList getStreamerFollowedList(QByteArray);

signals:

public slots:

private:
};

#endif // JSON_H
