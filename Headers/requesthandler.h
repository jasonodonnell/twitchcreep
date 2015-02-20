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
    void getFeatured(QByteArray data);
    void getFollows(QByteArray data);
    void getFollowsList(QByteArray data);
    void getGame(QByteArray data);
    void getStreamImage(QByteArray data, QString name);
    void getUsername(QByteArray data, QString name);

public:
    explicit requestHandler(QObject *parent = 0);
    ~requestHandler();

    void changeDisplayVariable(QString requestType, QString username);
    bool checkConnection();
    void checkUsername(QString text);
    void makeFollowRequest(QString username);
    void makeFeaturedRequest();
    void makeRequest();
    void makeSearchRequest(QString search);
    QByteArray readStreamImage(QString name);
    QList<QStringList> timedDatabaseRead(int index);
    QStringList timedOfflineRemoval(int index);

    QString getStatus(QString username);
    QList<QStringList> timedDatabaseUpdateRead(int index);
signals:
    void clearFollowList();
    void usernameDialogSignal(QString );

public slots:
    void requestProcess(QByteArray data, QString jsonType);

private:
    database db;
    json jsonParser;
    networkOperations networking;
    QSettings settings;
};

#endif // REQUESTHANDLER_H
