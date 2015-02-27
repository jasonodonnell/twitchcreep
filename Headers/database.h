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
    bool checkIfTopExists(QString game);
    QStringList getDisplayedOfflineStreams(QString requestType);
    void initTables();
    void manageDisplayVariable(QString requestType, QString username);
    void manageDisplayVariableClear(QString requestType);
    void manageOnlineStreamers(QString requestType);
    QString retrieveStatus(QString username, QString requestType);
    QList<QStringList> retreiveStreamList(QString requestType);
    QList<QStringList> retrieveTopList();
    QList<QStringList> retreiveUpdatedStreamList(QString requestType);
    void storeStreamData(QStringList streamData, QString requestType);
    void truncateStreamData();
signals:

private:
    void createTables();
};

#endif // DATABASE_H

