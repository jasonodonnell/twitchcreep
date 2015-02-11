#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QTimer>
#include <QObject>
#include <QSettings>

class networkOperations : public QObject
{
    Q_OBJECT
public:
    explicit networkOperations(QObject *parent = 0);
    ~networkOperations();

    void makeFeaturedRequest();
    void makeStreamRequest(QString);
    void makeFollowRequest(QString);
    bool checkNetworkConnection();
    void makeRequest(QString);
    void makeStreamImageRequest(QStringList);
    void makeStreamRequestFromList(QStringList usernames);
    void checkUsernameRequest(QString username);
    void makeGameRequest(QString game);
signals:
    void dataReady(QByteArray,QString);
public slots:
    void doneReading(QNetworkReply *reply);
private:
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
};

#endif // NETWORKOPERATIONS_H
