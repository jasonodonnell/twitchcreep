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
    qDebug() << data;
    jsonParser.determineDataSource(data);
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
   networking.makeFollowRequest("L0veWizard");

}

void MainWindow::on_pushButton_2_clicked()
{
    networking.makeFeaturedRequest();
}

void MainWindow::on_pushButton_3_clicked()
{
    networking.makeTopGamesRequest();
}

void MainWindow::on_pushButton_4_clicked()
{
    networking.makeStreamRequest("merlinidota");
}
