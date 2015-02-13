#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include <QThread>
#include "json.h"
#include "database.h"
#include "networkoperations.h"

class requestHandler : public QThread
{
    Q_OBJECT
    void getFollows(QByteArray data);
    void getFollowsList(QByteArray data);
    void getFeatured(QByteArray data);
    void getStreamImage(QByteArray data, QString name);
    void getUsername(QByteArray data, QString name);
    void getGame(QByteArray data);
public:
    explicit requestHandler(QObject *parent = 0);
    ~requestHandler();

    void checkUsername(QString text);
    void makeFollowRequest(QString username);
    void makeFeaturedRequest();
    QList<QStringList> timedDatabaseRead(int index);
    void makeSearchRequest(QString search);
    bool checkConnection();
    QByteArray readStreamImage(QString name);
    void makeStreamImageRequest(QStringList stream);
    QList<QStringList> getStreamListNoImage();
    void makeImageRequest(QStringList);
    void changeDisplayVariable(QString requestType, QString username);
    QStringList timedOfflineRemoval(int index);
    void makeRequest();
signals:
    void usernameDialogSignal(QString );
    void clearFollowList();

public slots:
    void requestProcess(QByteArray data, QString jsonType);

private:
    json jsonParser;
    networkOperations networking;
    database db;
    QSettings settings;
};

#endif // REQUESTHANDLER_H
