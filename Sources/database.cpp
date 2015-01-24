#include "database.h"

database::database(QObject *parent) : QObject(parent)
{
    this->initTables();
}

database::~database()
{

}

void database::initTables()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/Wizard/Git/TwitchCreep/twitch.db");
    qDebug() << "Test";
    qDebug() << db.open();
    if(db.open())
    {
        QSqlQuery query(db);
        if(query.exec("SELECT * FROM stream_data;"))
        {
            qDebug() << "Success";
            while(query.next())
            {
                QString username = query.value(1).toString();
                qDebug() << username;
            }
        }
        else
        {
            qDebug() << query.lastError();
        }
    }
    else
    {
        qDebug() << "Cannot open database";
    }
}

