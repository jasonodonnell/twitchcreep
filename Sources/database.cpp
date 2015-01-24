#include "database.h"

database::database(QObject *parent) : QObject(parent)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("/Users/Wizard/Git/TwitchCreep/twitch.db");
    this->initTables();
}

database::~database()
{

}

void database::initTables()
{
    if(checkDBConnection())
    {
        QSqlQuery query(this->db);
        if(!query.exec("DELETE FROM stream_data;"))
            qDebug() << query.lastError();
        else
            qDebug() << "Truncated";
        checkIfStreamExists("L0veWizard");
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

void database::storeStreamData(QStringList streamData, QString requestType)
{
    if(checkDBConnection())
    {
        QString username = streamData[0];
        QString game = "'" + streamData[1] + "'";
        QString viewers = streamData[2];
        QString status = "'" + streamData[3] + "'";
        QString logo = "'" + streamData[4] + "'";
        QString url = "'" + streamData[5] + "'";
        QString followed = "'false'";
        QString featured = "'false'";
        QString top = "'false'";
        if(requestType == "followed")
            followed = "'true'";
        else if(requestType == "featured")
            featured = "'true'";
        else if(requestType == "top")
            top = "'true'";
        if(!checkIfStreamExists(username))
        {
            username = "'" + streamData[0] + "',";
            QString queryString = "INSERT INTO stream_data (username, game, viewers, status, logo, url, followed, featured, top) ";
            queryString = queryString + "VALUES (" + username + game + "," + viewers + "," + status + "," + logo + "," + url + ",";
            queryString = queryString + followed + "," + featured + "," + top + ");";
            queryString = queryString.replace("'","\'");
            queryString = queryString.replace(":","\:");
            QSqlQuery query(this->db);
            if(!query.exec(queryString))
                qDebug() << query.lastError();
        }
        else
        {
            username = "'" + streamData[0] + "'";
            QString queryString = "UPDATE stream_data SET game = " + game + ',';
            queryString = queryString + "viewers = " + viewers + ',';
            queryString = queryString + "status = " +  status + ',';
            queryString = queryString + "followed = " + followed + ',';
            queryString = queryString + "featured = " + featured + ',';
            queryString = queryString + "top = " + top + ' ';
            queryString = queryString + "WHERE username=" + username;
            queryString = queryString.replace("'","\'");
            queryString = queryString.replace(":","\:");
            QSqlQuery query(this->db);
            if(!query.exec(queryString))
                qDebug() << query.lastError();
        }

    }
}

bool database::checkIfStreamExists(QString username)
{
    QString queryString = "SELECT count(username) FROM stream_data WHERE username='" + username + "';";
    QSqlQuery query(this->db);
    if(!query.exec(queryString))
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
    QString queryString;
    if(checkDBConnection())
    {
        if(requestType == "follow")
            queryString = "SELECT username,game,viewers,status,logo,url FROM stream_data WHERE followed='true';";
        else if(requestType == "featured")
            queryString = "SELECT username,game,viewers,status,logo,url FROM stream_data WHERE featured='true';";
        else if(requestType == "top")
            queryString = "SELECT username,game,viewers,status,logo,url FROM stream_data WHERE top='true';";

        QSqlQuery query(this->db);
        if(query.exec(queryString))
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
