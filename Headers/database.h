#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QDebug>
#include <QtSql>

class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    QSqlDatabase db;
    ~database();

    bool checkDBConnection();
    bool checkIfStreamExists(QString username, QString requestType);
    QStringList getDisplayedOfflineStreams(QString requestType);
    void initTables();
    QString retrieveStatus(QString username, QString requestType);
public slots:
    void manageOnlineStreamers(QString requestType);
    void removeOfflineStreamers();
    void storeItemIndex(QString requestType, QString username, int index);
    void storeStreamData(QStringList streamData, QString requestType);
    void truncateSearchData();
    void truncateStreamData();
signals:
    void addStreamToView(QStringList stream);
    void deleteStreamerFromListView(int itemIndex);
    void listViewClears();
    void updateStreamInView(QStringList stream,int index);
private:
    void createTables();
    int retrieveIndex(QString requestType, QString username);
};

#endif // DATABASE_H

