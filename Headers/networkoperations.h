#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>
#include <QStringList>

class networkOperations : public QObject
{
    Q_OBJECT
public:
    explicit networkOperations(QObject *parent = 0);
    ~networkOperations();

    bool checkNetworkConnection();
    void checkUsernameRequest(QString username);
    void makeFeaturedRequest();
    void makeFollowRequest(QString);
    void makeGameRequest(QString game);
    void makeRequest();
    void makeStreamRequest(QString);
    void makeStreamRequestFromList(QStringList usernames);

signals:
    void dataReady(QByteArray,QString);
    void removeOfflineStreamers();

public slots:
    void doneReading(QNetworkReply *reply);
    void popRequestFromList();

private:
    QString appId = "t4wqbkwgdox7xzbt6fd5ldwls37b6fh";
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QList<QStringList> requests;
    void addRequestToList(QString requestType, QString url);
};

#endif // NETWORKOPERATIONS_H
