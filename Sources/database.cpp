#include "../Headers/database.h"

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

//Check connection to the database before sql statements are prepared.
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

//Check if a stream exists in the database for a given requestType
bool database::checkIfStreamExists(QString username, QString requestType)
{
    bool returnCode = false;
    QSqlQuery query(this->db);
    if(requestType == "followed")
        query.prepare("SELECT username FROM followed_data WHERE username=:username");
    else if(requestType == "featured")
        query.prepare("SELECT username FROM featured_data WHERE username=:username");
    else if(requestType == "search")
        query.prepare("SELECT username FROM search_data WHERE username=:username");
    query.bindValue(":username",username);
    if(query.exec())
    {
        while(query.next())
        {
            if(!query.value( 0 ).toString().isNull())
                returnCode = true;
        }
    }
    return returnCode;
}

//Creates the table DDL for the app
void database::createTables()
{
    if(checkDBConnection())
    {
        QString drop_followed = "DROP TABLE IF EXISTS followed_data;";
        QString drop_featured = "DROP TABLE IF EXISTS featured_data;";
        QString drop_search = "DROP TABLE IF EXISTS search_data;";
        QString followed_data = "CREATE TABLE IF NOT EXISTS followed_data (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        followed_data.append("status STRING, logo STRING, url STRING, image BLOB, online BOOL, itemIndex INT);");
        QString featured_data = "CREATE TABLE IF NOT EXISTS featured_data (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        featured_data.append("status STRING, logo STRING, url STRING, image BLOB, online BOOL, itemIndex INT);");
        QString search_data = "CREATE TABLE IF NOT EXISTS search_data (oid INTEGER PRIMARY KEY, username STRING, game STRING, viewers INT, ";
        search_data.append("status STRING, logo STRING, url STRING, image BLOB, online BOOL, itemIndex INT);");

        QSqlQuery query(this->db);
        if(!query.exec(drop_followed))
            qDebug() << query.lastError();
        if(!query.exec(drop_featured))
            qDebug() << query.lastError();
        if(!query.exec(drop_search))
            qDebug() << query.lastError();
        if(!query.exec(followed_data))
            qDebug() << query.lastError();
        if(!query.exec(featured_data))
            qDebug() << query.lastError();
        if(!query.exec(search_data))
            qDebug() << query.lastError();
    }
}

//Gets a list of usernames that aren't online but are being displayed.  Upon a network
//request for streams, all streams currently online are changed to offline and will be
//updated to online by the request.  Whatever isn't online is no longer broadcasting
//and should be removed from views.
QStringList database::getDisplayedOfflineStreams(QString requestType)
{
    QSqlQuery query(this->db);
    QStringList streamData;
    if(requestType == "followed")
        query.prepare("SELECT username FROM followed_data WHERE online=0");
    else if(requestType == "featured")
        query.prepare("SELECT username FROM featured_data WHERE online=0");
    else if(requestType == "search")
        query.prepare("SELECT username FROM search_data WHERE online=0");

    if(checkDBConnection())
        if(query.exec())
            while(query.next())
                streamData << query.value(0).toString();
    return streamData;
}

//Intialize tables by creating the table and truncating any data.
void database::initTables()
{
    this->createTables();
    this->truncateStreamData();
}

//Clears online users when adding the updated data.  This is how we figure out who
//is now offline.
void database::manageOnlineStreamers(QString requestType)
{
    QString queryString;
    if(requestType == "followed")
        queryString = "UPDATE followed_data SET online=0";
    else if(requestType == "featured")
        queryString = "UPDATE featured_data SET online=0";
    else if(requestType == "search")
        queryString = "UPDATE search_data SET online=0";

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

//Retrieves the stream status for the tooltip.
int database::retrieveIndex(QString requestType, QString username)
{
    int indexValue = -1;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(requestType == "featured")
            query.prepare("SELECT itemIndex FROM featured_data WHERE username=:username");
        else if(requestType == "followed")
            query.prepare("SELECT itemIndex FROM followed_data WHERE username=:username");
        query.bindValue(":username",username);
        if(query.exec())
            while(query.next())
                indexValue = query.value(0).toInt();
    }
    return indexValue;
}

//Retrieves the stream status for the tooltip.
QString database::retrieveStatus(QString username,QString requestType)
{
    QString status;
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(requestType == "featured")
            query.prepare("SELECT status FROM featured_data WHERE username=:username");
        else if(requestType == "followed")
            query.prepare("SELECT status FROM followed_data WHERE username=:username");
        else if(requestType == "search")
            query.prepare("SELECT status FROM search_data WHERE username=:username");
        query.bindValue(":username",username);
        if(query.exec())
            while(query.next())
                return query.value(0).toString();
    }
    return status;
}

//Stores an index for a given item
void database::storeItemIndex(QString requestType, QString username, int index)
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        QString update;
        if(requestType == "followed")
            update = "UPDATE followed_data SET itemIndex = :index ";
        else if(requestType == "featured")
            update = "UPDATE featured_data SET itemIndex = :index ";
        QString values = "WHERE username = :username";

        QString queryString = update+values;
        query.prepare(queryString);
        query.bindValue(":index", index);
        query.bindValue(":username", username);

        if(!query.exec())
            qDebug() << query.lastError();
    }
}

//Stores stream data in the database.  Could possibly fire two signals: the add new item
//or update an existing item signals.
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
        int online = 1;

        QStringList streamData;
        streamData << username << game << viewers << requestType;

        if(!checkIfStreamExists(username,requestType))
        {
            QString insert;
            QSqlQuery query(this->db);
            if(requestType == "followed")
                insert = "INSERT INTO followed_data (username, game, viewers, status, logo, url, online) ";
            else if(requestType == "featured")
                insert = "INSERT INTO featured_data (username, game, viewers, status, logo, url, online) ";
            else if(requestType == "search")
                insert = "INSERT INTO search_data (username, game, viewers, status, logo, url, online) ";
            QString values = "VALUES (:username, :game, :viewers, :status, :logo, :url, :online)";
            QString queryString = insert + values;

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
            emit(addStreamToView(streamData));
        }
        else
        {
            QSqlQuery query(this->db);
            QString update;
            if(requestType == "followed")
                update = "UPDATE followed_data SET game = :game,";
            else if(requestType == "featured")
                update = "UPDATE featured_data SET game = :game,";
            else if(requestType == "search")
                update = "UPDATE search_data SET game = :game,";
            QString values = "viewers = :viewers, status = :status, online = :online WHERE username = :username";

            QString queryString = update+values;
            query.prepare(queryString);
            query.bindValue(":game", game);
            query.bindValue(":viewers", viewers);
            query.bindValue(":status", status);
            query.bindValue(":online", online);
            query.bindValue(":username", username);

            if(!query.exec())
                qDebug() << query.lastError();
            int index = this->retrieveIndex(requestType,username);
            emit(updateStreamInView(streamData,index));
        }
    }
}

void database::storeStreamImage(QByteArray data, QString username, QString requestType)
{
    if(checkDBConnection())
    {
        QString update = "";
        QSqlQuery query(this->db);
        if(requestType == "featured")
        {
            update = "UPDATE featured_data SET image = :image WHERE username = :username";
        }
        else if(requestType == "followed")
        {
            update = "UPDATE featured_data SET image = :image WHERE username = :username";
        }
        else if(requestType == "search")
        {
            update = "UPDATE featured_data SET image = :image WHERE username = :username";
        }
        query.prepare(update);
        query.bindValue(":image",data);
        query.bindValue(":username",username);
    }
}

//Truncates the stream data, used to clear the database upon startup.  This might get changed in the future
//to cache info.
void database::truncateStreamData()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM followed_data;"))
            qDebug() << query.lastError();
        if(!query.exec("DELETE FROM featured_data;"))
            qDebug() << query.lastError();
        if(!query.exec("DELETE FROM search_data;"))
            qDebug() << query.lastError();
        emit(listViewClears());
    }
}
