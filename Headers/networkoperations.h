#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
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
    bool checkNetworkConnection();
    void makeRequest(QString);
    void makeStreamImageRequest(QStringList);
    void makeTopImageRequest();
    void makeStreamRequestFromList(QStringList usernames);
    void makeTopImageRequest(QStringList topDataList);
    void checkUsernameRequest(QString username);
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
