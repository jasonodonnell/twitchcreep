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
    bool checkIfStreamExists(QString username, QString requestType);
    bool checkIfTopExists(QString game);
    void initTables();
    void getOfflineDisplayedStreams(QString requestType);
    void manageOnlineStreamers(QString requestType);
    QByteArray retrieveStreamImage(QString name);
    QList<QStringList> retreiveStreamList(QString requestType);
    QByteArray retrieveTopImage(QString game);
    QList<QStringList> retrieveTopList();
    QList<QStringList> retrieveTopListWithoutImage();
    QList<QStringList> retrieveStreamListWithoutImage();
    void storeStreamData(QStringList streamData, QString requestType);
    void storeImageFromUsername(QString name, QByteArray data);
    void storeTopData(QList<QStringList> topData);
    void truncateStreamData();
    void truncateTopData();
    void getDisplayedOfflineStreams(QString requestType);
    void getNonDisplayedOnlineStreams(QString requestType);
signals:

public slots:
    void storeImageFromTop(QString game, QByteArray data);
private:
    networkOperations networking;
    void createTables();
};

#endif // DATABASE_H
