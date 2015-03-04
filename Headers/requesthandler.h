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
    void makeFollowRequest(QString username);
    void makeFeaturedRequest();
    void makeRequest();
    void makeSearchRequest(QString search);
    QByteArray readStreamImage(QString name);
    void setSettingsValue(QString setting, QString value);
signals:
    void clearFollowList();
    void manageOnlineStreamers(QString);
    void storeStreamData(QStringList data, QString requestType);
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
