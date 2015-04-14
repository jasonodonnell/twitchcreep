#include "../Headers/networkoperations.h"

networkOperations::networkOperations(QObject *parent) : QObject(parent)
{
    //Signal-slot connection for network reply, when its finished, activates doneReading (passess the reply)
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(doneReading(QNetworkReply*)));
}

networkOperations::~networkOperations()
{

}

//Adds request to a queue, this is used to throttle requests going through the network managr
//to ensure data isn't confused.
void networkOperations::addRequestToList(QString requestType, QString url)
{
    QStringList requestList;
    if(!requestType.isEmpty() && !requestType.isEmpty())
    {
        requestList << requestType << url;
        requests << requestList;
    }
}

//Check current internet connection
bool networkOperations::checkNetworkConnection()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}

//Check if username exists in twitch.  This is used for checking username entered for
//followed query.
void networkOperations::checkUsernameRequest(QString username)
{
    if(!username.isEmpty())
    {
        QString url = "https://api.twitch.tv/kraken/users/" + username + "?client_id=" + appId;
        QString objectName = "usernameCheck:" + username;
        this->addRequestToList(objectName,url);
    }
}

//Slot that is activated after finished signal is fired.  Reads
//data into a byte array, sets the objectname and emits a signal.
void networkOperations::doneReading(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    QString requestType = this->objectName();
    emit(dataReady(replyData,requestType));
}

//Makes a featured stream request by adding a request to the request queue.
void networkOperations::makeFeaturedRequest()
{
    QString url = "https://api.twitch.tv/kraken/streams/featured?client_id=" + appId;
    this->addRequestToList("featured",url);
}

//Make the follow request based on username in QSettings
void networkOperations::makeFollowRequest(QString username)
{
    if(!username.isEmpty())
    {
        QString url = "https://api.twitch.tv/kraken/users/" + username + "/follows/channels?client_id=" + appId;
        this->addRequestToList("follows",url);
    }
}

//Gets list of streams for a game
void networkOperations::makeGameRequest(QString game)
{
    if(!game.isEmpty())
    {
        QString url = "https://api.twitch.tv/kraken/search/streams?q=" + game + "&client_id=" + appId;
        this->addRequestToList("game",url);
    }
}

//Makes the network request to twitch api
void networkOperations::makeRequest()
{
    if(requests.count() > 0)
    {
        if(this->checkNetworkConnection())
        {
            QStringList requestInfo = requests.first();
            this->setObjectName(requestInfo[0]);
            QString url = requestInfo[1];
            networkManager->get(QNetworkRequest(QUrl(url)));
            this->popRequestFromList();
        }
    }
}


//Make the stream request based on the username passed in
void networkOperations::makeStreamRequest(QString username)
{
    if(!username.isEmpty())
    {
        QString url = "https://api.twitch.tv/kraken/streams/" + username + "?client_id=" + appId;;
        this->addRequestToList("stream",url);
    }
}

//Makes the stream requests from a list of streamer usernames.
void networkOperations::makeStreamRequestFromList(QStringList usernames)
{
    if(!usernames.isEmpty())
    {
        foreach(QString username,usernames)
        {
            if(!username.isEmpty() && username != "End")
            {
                QString url = "https://api.twitch.tv/kraken/streams/" + username + "?client_id=" + appId;;
                this->addRequestToList("followsList",url);
            }
            else
                emit(removeOfflineStreamers());
        }
    }
}

//Used for managing the network request list.
void networkOperations::popRequestFromList()
{
    if(requests.count() > 0)
        requests.pop_front();
}
