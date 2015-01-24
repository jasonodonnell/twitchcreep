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
    ~database();

    void initTables();
signals:

public slots:
};

#endif // DATABASE_H
