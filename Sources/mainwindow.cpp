#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect((&networking),SIGNAL(dataReady(QByteArray)),this,SLOT(requestReady(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestReady(QByteArray data)
{
    QStringList streamers = streamsFollowed.getStreamersList(data);
    streamerFile.writeStreamersToDisk(streamers);
    streamerFile.readStreamersFromDisk();
}

void MainWindow::on_actionAdd_User_triggered()
{
    qDebug() << "File";
}

void MainWindow::on_actionExit_triggered()
{
    delete ui;
}
