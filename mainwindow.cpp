#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect((&streamsFollowed),SIGNAL(dataReady(QByteArray)),this,SLOT(streamersReady(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::streamersReady(QByteArray data)
{
    QStringList streamers = streamsFollowed.getStreamersList(data);
    streamerFile.writeStreamersToDisk(streamers);
}
