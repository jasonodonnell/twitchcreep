#include "mainwindow.h"
#include "ui_mainwindow.h"
QList<QStringList> followsList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    this->createSignalSlots();
    this->timedDataRequest();
    this->changeStatusBar();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createSignalSlots()
{
    //Signal-slot connection that is triggered by doneReading in networking after a web request is made.
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestReady(QByteArray,QString)));
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(timedDataRequest()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(timedDatabaseRead()));
    requestTimer->start(10000);
    readTimer->start(500);
}

//Slot that is called after data is finished reading from the network request.
//When the request is made, setObjectName is called so the sender can be identified.
void MainWindow::requestReady(QByteArray data, QString requestType)
{
    QString jsonType = requestType;
    if (jsonType == "follows")
    {
        QStringList follows;
        follows = jsonParser.getStreamerFollowedList(data);
        networking.makeStreamRequestFromList(follows);
    }
    else if (jsonType == "followList")
    {
        QStringList streamData = jsonParser.getStreamData(data);
        if(!streamData.isEmpty())
            db.storeStreamData(streamData, "followed");
    }
    else if (jsonType == "featured")
    {
        QList<QStringList> streamerList;
        QStringList streamData;
        streamerList << jsonParser.getFeaturedStreamData(data);
        foreach(streamData, streamerList)
            db.storeStreamData(streamData, "featured");
            networking.makeStreamImageRequest(streamData);
    }
    else if (jsonType == "stream")
    {
        QStringList streamer;
        streamer << jsonParser.getStreamData(data);
    }
    else if (jsonType == "top")
    {
        QList<QStringList> topGames;
        topGames << jsonParser.getTopGames(data);
        db.storeTopData(topGames);
    }
    else if (jsonType.contains("streamImage:"))
    {
        QStringList imageUsername = jsonType.split(":");
        QString name = imageUsername[1];
        db.storeImageFromUsername(name,data);

    }
    else if (jsonType.contains("topImage:"))
    {
        QStringList imageUsername = jsonType.split(":");
        QString name = imageUsername[1];
        db.storeImageFromTop(name,data);
    }
    else if (jsonType.contains("usernameCheck:"))
    {
        QStringList usernameCheck = jsonType.split(":");
        QString name = usernameCheck[1];
        bool exists;
        exists = jsonParser.checkUsernameExists(data);
        if(exists)
        {
            settings.setValue("username",name);
            db.truncateStreamData();
            db.truncateTopData();
            ui->listWidget_3->clear();
        }
        else
        {
            db.truncateStreamData();
            db.truncateTopData();
            settings.setValue("username","");
            ui->listWidget_3->clear();
            emit(usernameDialog("error"));
        }
    }
    else if (jsonType.contains("game"))
    {
        db.truncateStreamData();
        QList<QStringList> search = jsonParser.getGameStreamData(data);
        QStringList searchData;
        foreach(searchData, search)
            db.storeStreamData(searchData, "search");
            networking.makeStreamImageRequest(searchData);
    }
    else
        qDebug() << "Unknown data";
}

void MainWindow::usernameDialog(QString dialogType)
{
    bool ok;
    if(dialogType == "new")
    {
        bool ok;
        QString text = QInputDialog::getText(this,tr("Add Username"),tr("Username:"),QLineEdit::Normal,"", &ok);
        if(ok && !text.isEmpty())
            networking.checkUsernameRequest(text);
    }
    else if (dialogType == "error")
    {
        QString text = QInputDialog::getText(this,tr("Incorrect Username"),tr("Username:"),QLineEdit::Normal,"Username doesn't exist", &ok);
        if(ok && !text.isEmpty())
            networking.checkUsernameRequest(text);
    }
    else
        qDebug() << "Unknown data";
}

void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;

    if(tabIndex == 0)
        networking.makeFeaturedRequest();
    else if(tabIndex == 1)
        networking.makeTopGamesRequest();
    else if(tabIndex == 2)
    {
        if(!settings.value("username").isNull())
            networking.makeFollowRequest(settings.value("username").toString());
    }
}

void MainWindow::timedDatabaseRead()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;
    QList<QStringList> topDataList;

    if(tabIndex == 0){
        streamDataList = db.retreiveStreamList("featured");
        if(!streamDataList.isEmpty())
            this->addItemToListView(0,streamDataList);
    }
    else if(tabIndex == 1)
    {
        topDataList = db.retrieveTopList();
        if(!topDataList.isEmpty())
            this->addItemToListView(1,topDataList);
    }
    else if(tabIndex == 2)
    {
        streamDataList = db.retreiveStreamList("follow");
        if(!streamDataList.isEmpty())
            this->addItemToListView(2,streamDataList);
    }
    else if(tabIndex == 3)
    {
        streamDataList = db.retreiveStreamList("search");
        if(!streamDataList.isEmpty())
            this->addItemToListView(3,streamDataList);
    }
}

void MainWindow::on_actionAdd_User_triggered()
{
    emit(usernameDialog("new"));
}

void MainWindow::on_actionExit_triggered()
{
    delete ui;
}

void MainWindow::on_tabWidget_currentChanged()
{
    this->timedDataRequest();
}

//Adds items to the list view
void MainWindow::addItemToListView(int index, QList<QStringList> streams)
{
    if(index == 0)
    {
        ui->listWidget->clear();
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0];
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString status = streamData[3];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget->addItem(stream);
        }
        ui->listWidget->sortItems();
    }
    else if(index == 1)
    {
        ui->listWidget_2->clear();
        foreach (QStringList streamData, streams)
        {
            QString game = streamData[0];
            QString viewers = streamData[1];
            QString stream = game + ": (" + viewers + ") ";
            ui->listWidget_2->addItem(stream);
        }
        ui->listWidget_2->sortItems();
    }
    else if(index == 2)
    {
        ui->listWidget_3->clear();
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0];
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget_3->addItem(stream);
        }
        ui->listWidget_3->sortItems();
    }
    else if(index == 3)
    {
        ui->listWidget_4->clear();
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0];
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget_4->addItem(stream);
        }
    }
}

void MainWindow::changeStatusBar()
{
    if (networking.checkNetworkConnection())
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}

void MainWindow::searchTabRequest()
{
    QString search = ui->lineEdit->text();
    if(!search.isEmpty())
        networking.makeGameRequest(search);
}

void MainWindow::on_pushButton_pressed()
{
    this->searchTabRequest();
}

void MainWindow::on_lineEdit_returnPressed()
{
    this->searchTabRequest();
}
