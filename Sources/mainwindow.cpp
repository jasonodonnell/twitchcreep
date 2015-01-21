#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect((&networking),SIGNAL(dataReady(QByteArray,QString)),this,SLOT(requestReady(QByteArray,QString)));
    this->changeStatusBar();


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

void MainWindow::on_pushButton_clicked()
{
//    networking.makeStreamRequest("dotastarladder_en");
//    networking.makeStreamRequest("L0veWizard");
//    networking.makeFeaturedRequest();
//    networking.makeTopGamesRequest();
    networking.makeImageRequest("http://static-cdn.jtvnw.net/jtv_user_pictures/richard_hammer-profile_image-cbcf6eda2ff6fc2b-300x300.jpeg","richard_hammer");
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
