#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QDebug>
#include <QSql>
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

    void storeStreamImage(QByteArray data, QString username, QString requestType);
public slots:
    void manageOnlineStreamers(QString requestType);
    void storeItemIndex(QString requestType, QString username, int index);
    void storeStreamData(QStringList streamData, QString requestType);
    void truncateStreamData();
signals:
    void addStreamToView(QStringList stream);
    void listViewClears();
    void updateStreamInView(QStringList stream,int index);

private:
    void createTables();
    int retrieveIndex(QString requestType, QString username);
};

#endif // DATABASE_H

