#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QDebug>
#include <QSql>
#include <QtSql>

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
signals:

public slots:
};

#endif // DATABASE_H
