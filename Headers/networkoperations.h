#ifndef NETWORKOPERATIONS_H
#define NETWORKOPERATIONS_H
#include <QNetworkAccessManager>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QTimer>
#include <QObject>
#include <QSettings>
#include <QStringList>

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
    void makeRequest();
    void makeStreamImageRequest(QStringList);
    void makeStreamRequestFromList(QStringList usernames);
    void checkUsernameRequest(QString username);
    void makeGameRequest(QString game);

signals:
    void dataReady(QByteArray,QString);
public slots:
    void doneReading(QNetworkReply *reply);
    void popRequestFromList();
private:
    void addRequestToList(QString requestType, QString url);
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QList<QStringList> requests;
};

#endif // NETWORKOPERATIONS_H
