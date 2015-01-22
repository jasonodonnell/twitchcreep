#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestReady(QByteArray,QString)));
    this->tabRequest(ui->tabWidget->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestReady(QByteArray data, QString requestType)
{
    QString jsonType = requestType;
    if (jsonType == "follows")
    {
        QStringList follows;
        follows = jsonParser.getStreamerFollowedList(data);
        follows;
    }
    else if (jsonType == "featured")
    {
        QList<QStringList> streamerList;
        streamerList = jsonParser.getFeaturedStreamData(data);
        addItemToListView(0,streamerList);
    }
    else if (jsonType == "stream")
    {
        QStringList streamer;
        streamer << jsonParser.getStreamData(data);
        qDebug() << streamer;
    }
    else if (jsonType == "top")
    {
        QList<QStringList> topGames;
        topGames << jsonParser.getTopGames(data);
        qDebug() << topGames;
    }
    else if (jsonType.contains("image:"))
    {
        QStringList imageFilename = jsonType.split(":");
        QString saveDir = "/Users/Wizard/" + imageFilename[1];
        image.saveScaledImage(saveDir, data, 30, 30);
    }
    else
    {
        qDebug() << "Unknown data";
    }
}

void MainWindow::on_actionAdd_User_triggered()
{
    qDebug() << "File";
}

void MainWindow::on_actionExit_triggered()
{
    delete ui;
}

void MainWindow::tabRequest(int index)
{
    QString username = "L0veWizard";
    if (index == 0)
    {
        networking.makeFeaturedRequest();
    }
    else if (index == 1)
    {
        networking.makeTopGamesRequest();
    }
    else if (index == 2)
    {
        networking.makeFollowRequest(username);
    }
    else if (index == 3)
    {
        networking.makeStreamRequest(username);
    }
    else
    {
        qDebug() << "I don't know this index";
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Button";
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    this->tabRequest(index);
}

void MainWindow::addItemToListView(int index, QList<QStringList> streams)
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

void MainWindow::changeStatusBar()
{
    int connection = networking.checkNetworkConnection();
    if (connection == 0)
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}
