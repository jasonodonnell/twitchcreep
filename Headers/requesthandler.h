#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include "json.h"
#include "networkoperations.h"

class requestHandler : public QObject
{
    Q_OBJECT
    void getFeatured(QByteArray data);
    void getFollows(QByteArray data);
    void getFollowsList(QByteArray data);
    void getGame(QByteArray data);
    void getUsername(QByteArray data, QString name);

public:
    explicit requestHandler(QObject *parent = 0);
    ~requestHandler();
    QString getSettingsValue(QString value);
    bool checkConnection();
    bool checkForLiveStreamer();
    void checkUsername(QString text);
    void makeFollowRequest(QString username);
    void makeFeaturedRequest();
    void makeRequest();
    void makeSearchRequest(QString search);
    void setSettingsValue(QString setting, QString value);
    void storeItemIndex(QString requestType, QString username, int index);
signals:
    void clearFollowList();
    void clearSearchList();
    void deleteOfflineStreamersFromDB();
    void manageOnlineStreamers(QString);
    void storeIndex(QString requestType, QString username, int index);
    void storeStreamData(QStringList data, QString requestType);
    void truncateSearchData();
    void truncateStreamData();
    void usernameDialogSignal(QString);

public slots:
    void requestProcess(QByteArray data, QString jsonType);

private slots:
    void removeOfflineStreamers();
private:
    json jsonParser;
    networkOperations networking;
};

#endif // REQUESTHANDLER_H
