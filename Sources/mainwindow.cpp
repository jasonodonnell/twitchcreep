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
    QStringList streamers;
    QString jsonType = jsonParser.determineDataSource(data);
    qDebug() << jsonType;

    if (jsonType == "follows")
        streamers = jsonParser.getStreamerFollowedList(data);
    else if (jsonType == "featured")
        jsonParser.getFeaturedStreamData(data);
    else if (jsonType == "stream")
        jsonParser.getStreamData(data);
    else
        qDebug() << "Not sure what kind of data this is.";
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
    networking.makeStreamRequest("ppd");
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
