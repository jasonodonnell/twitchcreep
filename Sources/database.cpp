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

void database::createTables()
{
    if(checkDBConnection())
    {
        QString drop_featured = "DROP TABLE IF EXISTS featured;";
        QString drop_followed = "DROP TABLE IF EXISTS followed;";
        QString drop_search = "DROP TABLE IF EXISTS search;";
        QString drop_top = "DROP TABLE IF EXISTS top_data;";
        QString featured = "CREATE TABLE IF NOT EXISTS featured (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        featured.append("status STRING, logo STRING, url STRING, online BOOL, displayed BOOL);");
        QString followed = "CREATE TABLE IF NOT EXISTS followed (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        followed.append("status STRING, logo STRING, url STRING, online BOOL, displayed BOOL);");
        QString search = "CREATE TABLE IF NOT EXISTS search (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        search.append("status STRING, logo STRING, url STRING, online BOOL, displayed BOOL);");
        QString featured_unique = "CREATE UNIQUE INDEX featured_unique ON featured (username)";
        QString followed_unique = "CREATE UNIQUE INDEX followed_unique ON followed (username)";
        QString search_unique = "CREATE UNIQUE INDEX search_unique ON search (username)";

        QSqlQuery query(this->db);
        if(!query.exec(drop_featured))
            qDebug() << query.lastError();
        if(!query.exec(drop_followed))
            qDebug() << query.lastError();
        if(!query.exec(drop_search))
            qDebug() << query.lastError();
        if(!query.exec(drop_top))
            qDebug() << query.lastError();
        if(!query.exec(featured))
            qDebug() << query.lastError();
        if(!query.exec(followed))
            qDebug() << query.lastError();
        if(!query.exec(search))
            qDebug() << query.lastError();
        if(!query.exec(followed_unique))
            qDebug() << query.lastError();
        if(!query.exec(featured_unique))
            qDebug() << query.lastError();
        if(!query.exec(search_unique))
            qDebug() << query.lastError();
    }
}

QStringList database::getDisplayedOfflineStreams(QString requestType)
{
    QSqlQuery query(this->db);
    QStringList streamData;
    QString queryString = "SELECT username FROM " + requestType + " WHERE online='false' AND displayed='true'";
    query.prepare(queryString);

    if(checkDBConnection())
        if(query.exec())
            while(query.next())
                streamData << query.value(0).toString();
    return streamData;
}

void database::initTables()
{
    this->createTables();
}

void database::manageDisplayVariable(QString requestType, QString username)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);

        QString queryString = "UPDATE " + requestType + " SET displayed='true' WHERE username=:username";
        query.prepare(queryString);
        query.bindValue(":username",username);
        if(!query.exec())
            qDebug() << query.lastError();
    }
}

void database::manageOnlineStreamers(QString requestType)
{
    QSqlQuery query(this->db);
    QString queryString;
    if(requestType == "featured")
        queryString = "UPDATE featured SET online='false';";
    else if(requestType == "followed")
        queryString = "UPDATE followed SET online='false';";
    else if(requestType == "search")
        queryString = "UPDATE search SET online='false';";
    qDebug() << queryString;
    query.prepare(queryString);
    if(checkDBConnection())
    {
        if(!query.exec())
            qDebug() << query.lastError();
    }
    qDebug() << "End";
}

QList<QStringList> database::retreiveStreamList(QString requestType)
{
    QList<QStringList> streamDataList;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        QString queryString = "SELECT username,game,viewers,status,logo,url FROM " + requestType + " WHERE displayed IS NULL";
        query.prepare(queryString);

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
        QString username = streamData[0];
        QString game = streamData[1];
        QString viewers = streamData[2];
        QString status = streamData[3];
        QString logo = streamData[4];
        QString url = streamData[5];
        QString online = "true";

        QSqlQuery query(this->db);
        QString update = "INSERT OR REPLACE INTO " + requestType +  " (username,game,viewers,status,logo,url,online) ";
        QString values = "VALUES (:username,:game,:viewers,:status,:logo,:url,:online);";
        QString queryString = update+values;
        query.prepare(queryString);
        query.bindValue(":username", username);
        query.bindValue(":game", game);
        query.bindValue(":viewers", viewers);
        query.bindValue(":status", status);
        query.bindValue(":logo", logo);
        query.bindValue(":url", url);
        query.bindValue(":online", online);

        if(!query.exec())
            qDebug() << query.lastError();
    }
}

void database::truncateFollowData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM followed;"))
            qDebug() << query.lastError();
    }
}

void database::truncateFeaturedData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM featured;"))
            qDebug() << query.lastError();
    }
}

void database::truncateSearchData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM search;"))
            qDebug() << query.lastError();
    }
}
