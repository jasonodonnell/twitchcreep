#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include <QObject>

class networkOperations : public QObject
{
    Q_OBJECT
public:
    explicit networkOperations(QObject *parent = 0);
    ~networkOperations();

    void makeRequest(QString url); 
    void makeTopGamesRequest();
    void makeFeaturedRequest();
    void makeStreamRequest(QString);
    void makeFollowRequest(QString);
signals:
    void dataReady(QByteArray);
public slots:
    void doneReading(QNetworkReply *reply);
    void timedFollowRequest();
private:
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QTimer *timer = new QTimer(this);
};

#endif // NETWORKOPERATIONS_H
