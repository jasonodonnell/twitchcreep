#include "database.h"

database::database(QObject *parent) : QObject(parent)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    QString appDir = QCoreApplication::applicationDirPath();
    appDir = appDir.append("/twitch.db");
    this->db.setDatabaseName(appDir);
    this->initTables();
}

database::~database()
{

}

void database::initTables()
{
    this->createTables();
    this->truncateStreamData();
    this->truncateTopData();
}

void database::createTables()
{
    if(checkDBConnection())
    {
        QString top_data = "CREATE TABLE IF NOT EXISTS top_data (oid INTEGER PRIMARY KEY, game STRING, viewers INT, logo STRING, image BLOB);";
        QString stream_data = "CREATE TABLE IF NOT EXISTS stream_data (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        stream_data.append("status STRING, logo STRING, url STRING, image BLOB, followed BOOL, featured BOOL, top BOOL, search BOOL);");

        QSqlQuery query(this->db);
        if(!query.exec(top_data))
            qDebug() << query.lastError();
        if(!query.exec(stream_data))
            qDebug() << query.lastError();
    }
}

bool database::checkDBConnection()
{
    if(this->db.open())
        return true;
    else
    {
        qDebug() << this->db.lastError();
        return false;
    }
}

void database::truncateStreamData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM stream_data;"))
            qDebug() << query.lastError();
    }
}

void database::truncateTopData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM top_data;"))
            qDebug() << query.lastError();
    }
}


void database::storeTopData(QList<QStringList> topData)
{
    if(checkDBConnection())
    {
        foreach(QStringList top, topData)
        {
            QString game = top[0];
            QString viewers = top[1];
            QString logo = top[2];

            if(!checkIfTopExists(game))
            {
                QSqlQuery query(this->db);
                query.prepare("INSERT INTO top_data (game, viewers, logo) VALUES (:game, :viewers, :logo)");
                query.bindValue(":game",game);
                query.bindValue(":viewers",viewers);
                query.bindValue(":logo",logo);
                if(!query.exec())
                    qDebug() << query.lastError();
            }
            else
            {
                QSqlQuery query(this->db);
                query.prepare("UPDATE top_data SET game = :game, viewers = :viewers, logo = :logo WHERE game = :game");
                query.bindValue(":game",game);
                query.bindValue(":viewers",viewers);
                query.bindValue(":logo",logo);
                if(!query.exec())
                    qDebug() << query.lastError();
            }
        }
    }
}

void database::storeStreamData(QStringList streamData, QString requestType)
{
    if(checkDBConnection())
    {
        QString username = streamData[0];
        QString game = streamData[1];
        QString viewers = streamData[2];
        QString status = streamData[3];
        QString logo = streamData[4];
        QString url = streamData[5];
        QString followed = "false";
        QString featured = "false";
        QString top = "false";
        QString search = "false";

        if(requestType == "followed")
            followed = "true";
        else if(requestType == "featured")
            featured = "true";
        else if(requestType == "top")
            top = "true";
        else if(requestType == "search")
            search = "true";
        if(!checkIfStreamExists(username))
        {
            QSqlQuery query(this->db);
            QString insert = "INSERT INTO stream_data (username, game, viewers, status, logo, url, followed, featured, top, search) ";
            QString values = "VALUES (:username, :game, :viewers, :status, :logo, :url, :followed, :featured, :top, :search)";
            QString queryString = insert + values;

            query.prepare(queryString);
            query.bindValue(":username", username);
            query.bindValue(":game", game);
            query.bindValue(":viewers", viewers);
            query.bindValue(":status", status);
            query.bindValue(":logo", logo);
            query.bindValue(":url", url);
            query.bindValue(":followed", followed);
            query.bindValue(":featured", featured);
            query.bindValue(":top", top);
            query.bindValue(":search", search);

            if(!query.exec())
                qDebug() << query.lastError();
        }
        else
        {
            QSqlQuery query(this->db);
            QString update = "UPDATE stream_data SET game = :game,";
            QString values = "viewers = :viewers, status = :status, followed = :followed, ";
            QString valuesCont = "featured = :featured, top = :top, search = :search WHERE USERNAME = :username";
            QString queryString = update+values+valuesCont;
            query.prepare(queryString);
            query.bindValue(":game", game);
            query.bindValue(":viewers", viewers);
            query.bindValue(":status", status);
            query.bindValue(":followed", followed);
            query.bindValue(":featured", featured);
            query.bindValue(":top", top);
            query.bindValue(":search", search);
            query.bindValue(":username", username);

            if(!query.exec())
                qDebug() << query.lastError();
        }
    }
}

bool database::checkIfStreamExists(QString username)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT count(username) FROM stream_data WHERE username=:username");
    query.bindValue(":username",username);
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toInt() > 0)
                return true;
            else
                return false;
        }
    }
}

bool database::checkIfTopExists(QString game)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT count(game) FROM top_data WHERE game=:game");
    query.bindValue(":game",game);
    if(!query.exec())
    {
        qDebug() << query.lastError();
        return false;
    }
    else
    {
        while(query.next())
        {
            if(query.value(0).toInt() > 0)
                return true;
            else
                return false;
        }
    }
}


QList<QStringList> database::retreiveStreamList(QString requestType)
{
    QList<QStringList> streamDataList;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(requestType == "follow")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE followed='true'");
        else if(requestType == "featured")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE featured='true'");
        else if(requestType == "top")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE top='true'");
        else if(requestType == "search")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE search='true'");

        if(query.exec())
        {
            while(query.next())
            {
                QStringList streamData;
                streamData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
                streamData << query.value(3).toString() << query.value(4).toString() << query.value(5).toString();
                streamDataList << streamData;
            }
        }
    }
    return streamDataList;
}

QList<QStringList> database::retrieveTopList()
{
    QList<QStringList> streamDataList;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        query.prepare("SELECT game,viewers,logo FROM top_data");
        if(query.exec())
        {
            while(query.next())
            {
                QStringList streamData;
                streamData << query.value(0).toString() << query.value(1).toString() << query.value(2).toString();
                streamDataList << streamData;
            }
        }
    }
    return streamDataList;
}



QList<QStringList> database::retrieveTopListWithoutImage()
{
    QList<QStringList> streamDataList;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        query.prepare("SELECT game,logo FROM top_data WHERE image is null or image = ''");

        if(query.exec())
        {
            while(query.next())
            {
                QStringList streamData;
                streamData << query.value(0).toString() << query.value(1).toString();
                streamDataList << streamData;
            }
        }
    }
    return streamDataList;
}

QList<QStringList> database::retrieveStreamListWithoutImage()
{
    QList<QStringList> streamDataList;
    QString queryString;
    if(checkDBConnection())
    {
        queryString = "SELECT username,logo FROM stream_data WHERE image is null or image = ''";

        QSqlQuery query(this->db);
        if(query.exec(queryString))
        {
            while(query.next())
            {
                QStringList streamData;
                streamData << query.value(0).toString() << query.value(1).toString();
                streamDataList << streamData;
            }
        }
    }
    return streamDataList;
}

void database::storeImageFromUsername(QString name, QByteArray data)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        query.prepare("UPDATE stream_data SET image = :image WHERE username = :name");
        query.bindValue(":image",data);
        query.bindValue(":name",name);
        if(!query.exec())
            qDebug() << query.lastError();
    }
}

void database::storeImageFromTop(QString game, QByteArray data)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        query.prepare("UPDATE top_data SET image = :image WHERE username = :game");
        query.bindValue(":image",data);
        query.bindValue(":name",game);
        if(!query.exec())
            qDebug() << query.lastError();
    }
}

