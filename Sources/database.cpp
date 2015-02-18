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

bool database::checkIfStreamExists(QString username, QString requestType)
{
    QSqlQuery query(this->db);
    if(requestType == "followed")
        query.prepare("SELECT username FROM stream_data WHERE username=:username AND followed=1");
    else if(requestType == "featured")
        query.prepare("SELECT username FROM stream_data WHERE username=:username AND featured=1");
    else if(requestType == "search")
        query.prepare("SELECT username FROM stream_data WHERE username=:username AND search=1");
    else
        query.prepare("SELECT username FROM stream_data WHERE username=:username");
    query.bindValue(":username",username);
    if(query.exec())
    {
        while(query.next())
        {
            if(!query.value( 0 ).toString().isNull())
                return true;
            else
                return false;
        }
    }
}

void database::createTables()
{
    if(checkDBConnection())
    {
        QString drop_stream = "DROP TABLE IF EXISTS stream_data;";
        QString drop_top = "DROP TABLE IF EXISTS top_data;";
        QString stream_data = "CREATE TABLE IF NOT EXISTS stream_data (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        stream_data.append("status STRING, logo STRING, url STRING, image BLOB, followed BOOL, featured BOOL, search BOOL, online BOOL, displayed BOOL);");

        QSqlQuery query(this->db);
        if(!query.exec(drop_stream))
            qDebug() << query.lastError();
        if(!query.exec(drop_top))
            qDebug() << query.lastError();
        if(!query.exec(stream_data))
            qDebug() << query.lastError();
    }
}

QStringList database::getDisplayedOfflineStreams(QString requestType)
{
    QSqlQuery query(this->db);
    QStringList streamData;
    if(requestType == "followed")
        query.prepare("SELECT username FROM stream_data WHERE followed=1 AND online=0 AND displayed=1");
    else if(requestType == "featured")
        query.prepare("SELECT username FROM stream_data WHERE featured=1 AND online=0 AND displayed=1");
    else if(requestType == "search")
        query.prepare("SELECT username FROM stream_data WHERE search=1 AND online=0 AND displayed=1");


    if(checkDBConnection())
        if(query.exec())
            while(query.next())
                streamData << query.value(0).toString();
    return streamData;
}

void database::initTables()
{
    this->createTables();
    this->truncateStreamData();
}

void database::manageDisplayVariable(QString requestType, QString username)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(requestType == "followed")
            query.prepare("UPDATE stream_data SET displayed=1 WHERE username=:username AND followed=1");
        else if(requestType == "featured")
            query.prepare("UPDATE stream_data SET displayed=1 WHERE username=:username AND featured=1");
        else if(requestType == "search")
            query.prepare("UPDATE stream_data SET displayed=1 WHERE username=:username AND search=1");

        query.bindValue(":username",username);
        if(!query.exec())
            qDebug() << query.lastError();
    }
}

void database::manageOnlineStreamers(QString requestType)
{
    QString queryString;
    if(requestType == "followed")
        queryString = "UPDATE stream_data SET online=0 WHERE followed=1";
    else if(requestType == "featured")
        queryString = "UPDATE stream_data SET online=0 WHERE featured=1";
    else if(requestType == "search")
        queryString = "UPDATE stream_data SET online=0 WHERE search=1";

    if(!queryString.isEmpty())
    {
        if(checkDBConnection())
        {
            QSqlQuery query(this->db);
            query.prepare(queryString);
            if(!query.exec())
                qDebug() << query.lastError();
        }
    }
}

QString database::retrieveStatus(QString username)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        query.prepare("SELECT status FROM stream_data WHERE username=:username");
        query.bindValue(":username",username);
        if(query.exec())
            while(query.next())
                return query.value(0).toString();
    }
}

QList<QStringList> database::retreiveStreamList(QString requestType)
{
    QList<QStringList> streamDataList;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(requestType == "follow")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE followed=1 AND online=1 AND displayed=0");
        else if(requestType == "featured")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE featured=1 AND online=1 AND displayed=0");
        else if(requestType == "search")
            query.prepare("SELECT username,game,viewers,status,logo,url FROM stream_data WHERE search=1 AND online=1 AND displayed=0");

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

void database::storeStreamData(QStringList streamData, QString requestType)
{
    if(checkDBConnection() && streamData[0] != "")
    {
        QString username = streamData[0].replace(" ","");
        QString game = streamData[1];
        QString viewers = streamData[2];
        QString status = streamData[3];
        QString logo = streamData[4];
        QString url = streamData[5];
        int followed = 0;
        int featured = 0;
        int search = 0;
        int online = 1;
        int displayed = 0;

        if(requestType == "followed")
            followed = 1;
        else if(requestType == "featured")
            featured = 1;
        else if(requestType == "search")
            search = 1;

        if(!checkIfStreamExists(username,requestType))
        {
            QSqlQuery query(this->db);
            QString insert = "INSERT INTO stream_data (username, game, viewers, status, logo, url, followed, featured, search, online, displayed) ";
            QString values = "VALUES (:username, :game, :viewers, :status, :logo, :url, :followed, :featured, :search, :online, :displayed)";
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
            query.bindValue(":search", search);
            query.bindValue(":online", online);
            query.bindValue(":displayed", displayed);

            if(!query.exec())
                qDebug() << query.lastError();
        }
        else
        {
            QSqlQuery query(this->db);
            QString update = "UPDATE stream_data SET game = :game,";
            QString values = "viewers = :viewers, status = :status, followed = :followed, ";
            QString valuesCont = "featured = :featured, search = :search, online = :online ";
            QString where;
            if(requestType == "followed")
                where = "WHERE username = :username AND followed=1";
            else if(requestType == "featured")
                where = "WHERE username = :username AND featured=1";
            else if(requestType == "search")
                where = "WHERE username = :username AND search=1";
            QString queryString = update+values+valuesCont+where;
            query.prepare(queryString);
            query.bindValue(":game", game);
            query.bindValue(":viewers", viewers);
            query.bindValue(":status", status);
            query.bindValue(":followed", followed);
            query.bindValue(":featured", featured);
            query.bindValue(":search", search);
            query.bindValue(":online", online);
            query.bindValue(":username", username);

            if(!query.exec())
                qDebug() << query.lastError();
        }
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
