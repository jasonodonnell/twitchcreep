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

    void makeTopGamesRequest();
    void makeFeaturedRequest();
    void makeStreamRequest(QString);
    void makeFollowRequest(QString);
    int checkNetworkConnection();
    void makeRequest(QString url);
    void makeImageRequest(QString,QString);
signals:
    void dataReady(QByteArray,QString);
public slots:
    void doneReading(QNetworkReply *reply);
    void timedFollowRequest();
private:
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QTimer *timer = new QTimer(this);
};

#endif // NETWORKOPERATIONS_H
