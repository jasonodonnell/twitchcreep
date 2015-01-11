#ifndef JSONRETRIEVER_H
#define JSONRETRIEVER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QNetworkReply>
#include <QTimer>


class json : public QObject
{
    Q_OBJECT
public:
    explicit json(QObject *parent = 0);
    QStringList getStreamersList(QByteArray);
    void makeRequest(QString url);
    ~json();

signals:
    void dataReady(QByteArray);

public slots:
    void doneReading(QNetworkReply *reply);
    void timedFollowRequest();

private:
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QTimer *timer = new QTimer(this);
};

#endif // JSON_H
