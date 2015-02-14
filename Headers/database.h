#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QDebug>
#include <QSql>
#include <QtSql>
#include "networkoperations.h"

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    QSqlDatabase db;
    ~database();

    bool checkDBConnection();
    QStringList getDisplayedOfflineStreams(QString requestType);
    void initTables();
    void manageDisplayVariable(QString requestType, QString username);
    void manageOnlineStreamers(QString requestType);
    QByteArray retrieveStreamImage(QString name);
    QList<QStringList> retreiveStreamList(QString requestType);
    QByteArray retrieveTopImage(QString game);
    QList<QStringList> retrieveTopList();
    QList<QStringList> retrieveTopListWithoutImage();
    QList<QStringList> retrieveStreamListWithoutImage();
    void storeStreamData(QStringList streamData, QString requestType);
    void truncateSearchData();
    void truncateFeaturedData();
    void truncateFollowData();

signals:

private:
    networkOperations networking;
    void createTables();
};

#endif // DATABASE_H
