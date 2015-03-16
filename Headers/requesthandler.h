#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include "json.h"
#include "networkoperations.h"

class requestHandler : public QThread
{
    Q_OBJECT
    void getFeatured(QByteArray data);
    void getFollows(QByteArray data);
    void getFollowsList(QByteArray data);
    void getGame(QByteArray data);
    void getStreamImage(QByteArray data, QString name);
    void getUsername(QByteArray data, QString name);

public:
    explicit requestHandler(QObject *parent = 0);
    ~requestHandler();

    bool checkConnection();
    void checkUsername(QString text);
    QString getSettingsValue(QString value);
    void getStreamImage(QByteArray data, QString username, QString requestType);
    void makeFollowRequest(QString username);
    void makeFeaturedRequest();
    void makeRequest();
    void makeSearchRequest(QString search);
    void makeStreamImageRequest(QString requestType, QString url, QString username);
    QByteArray readStreamImage(QString name);
    void setSettingsValue(QString setting, QString value);
    void storeItemIndex(QString requestType, QString username, int index);

signals:
    void clearFollowList();
    void manageOnlineStreamers(QString);
    void storeIndex(QString requestType, QString username, int index);
    void storeStreamData(QStringList data, QString requestType);
    void storeStreamImageData(QByteArray data, QString requestType, QString username);
    void truncateStreamData();
    void usernameDialogSignal(QString);

public slots:
    void requestProcess(QByteArray data, QString jsonType);

private:
    json jsonParser;
    networkOperations networking;
    QSettings settings;
};

#endif // REQUESTHANDLER_H
