#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QObject>
#include "json.h"
#include "database.h"
#include "networkoperations.h"

class requestHandler : public QObject
{
    Q_OBJECT
    void getFollows(QByteArray data);
    void getFollowsList(QByteArray data);
    void getFeatured(QByteArray data);
    void getTop(QByteArray data);
    void getStreamImage(QByteArray data, QString name);
    void getTopImage(QByteArray data, QString name);
    void getUsername(QByteArray data, QString name);
    void getGame(QByteArray data);
public:
    explicit requestHandler(QObject *parent = 0);
    ~requestHandler();

    void checkUsername(QString text);
    void makeFollowRequest(QString username);
    void makeTopRequest();
    void makeFeaturedRequest();
    QList<QStringList> timedDatabaseRead(int index);
    void makeSearchRequest(QString search);
    bool checkConnection();
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