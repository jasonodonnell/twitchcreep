#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect((&networking),SIGNAL(dataReady(QByteArray)),this,SLOT(requestReady(QByteArray)));
    this->changeStatusBar();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestReady(QByteArray data)
{

    QString jsonType = jsonParser.determineDataSource(data);

    if (jsonType == "follows")
    {
        QStringList follows;
        follows = jsonParser.getStreamerFollowedList(data);
        qDebug() << follows;
    }
    else if (jsonType == "featured")
    {
        QList<QStringList> streamerList;
        streamerList << jsonParser.getFeaturedStreamData(data);
        qDebug() << streamerList;
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
    else if (jsonType == "image")
    {
        qDebug() << "Is image";
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

void MainWindow::on_pushButton_clicked()
{
//    networking.makeStreamRequest("dotastarladder_en");
//    networking.makeStreamRequest("L0veWizard");
//    networking.makeFeaturedRequest();
//    networking.makeFollowRequest("L0veWizard");
//    networking.makeTopGamesRequest();
    networking.makeRequest("http://static-cdn.jtvnw.net/jtv_user_pictures/richard_hammer-profile_image-cbcf6eda2ff6fc2b-300x300.jpeg");
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    qDebug() << index;
}

void MainWindow::changeStatusBar()
{
    int connection = networking.checkNetworkConnection();
    if (connection == 0)
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}
