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
    bool checkIfTopExists(QString game);
public:
    explicit database(QObject *parent = 0);
    QSqlDatabase db;
    ~database();

    void initTables();
    void storeStreamData(QStringList streamData, QString requestType);
    bool checkDBConnection();
    bool checkIfStreamExists(QString username);
    QList<QStringList> retreiveStreamList(QString requestType);
    void storeTopData(QList<QStringList> topData);
    QList<QStringList> retrieveTopList();
    QList<QStringList> retrieveStreamListWithoutImage();
    QList<QStringList> retrieveTopListWithoutImage();
    void storeImageFromUsername(QString name, QByteArray data);
    void truncateStreamData();
    void truncateTopData();
signals:

public slots:
    void storeImageFromTop(QString game, QByteArray data);
private:
    networkOperations networking;
    void createTables();
};

#endif // DATABASE_H
