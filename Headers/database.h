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

public slots:
    void manageOnlineStreamers(QString requestType);
    void storeStreamData(QStringList streamData, QString requestType);
    void truncateStreamData();

signals:
    void addStreamToView(QStringList stream);
    void listViewClears();
    void updateStreamInView(QStringList stream);


private:
    void createTables();
};

#endif // DATABASE_H

