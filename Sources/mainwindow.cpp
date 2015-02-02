#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect((&request),SIGNAL(usernameDialogSignal(QString)),this,SLOT(usernameDialog(QString)));
    connect((&request),SIGNAL(clearFollowList()),this,SLOT(followListClear()));
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(timedDataRequest()));
    connect(readTimer,SIGNAL(timeout()),this,SLOT(timedDatabaseRead()));
    requestTimer->start(10000);
    readTimer->start(500);
}


void MainWindow::usernameDialog(QString dialogType)
{
    bool ok;
    if(dialogType == "new")
    {
        QString text = QInputDialog::getText(this,tr("Add Username"),tr("Username:"),QLineEdit::Normal,"", &ok);
        if(ok && !text.isEmpty())
            request.checkUsername(text);
    }
    else if (dialogType == "error")
    {
        QString text = QInputDialog::getText(this,tr("Incorrect Username"),tr("Username:"),QLineEdit::Normal,"Username doesn't exist", &ok);
        if(ok && !text.isEmpty())
            request.checkUsername(text);
    }
}

void MainWindow::followListClear()
{
    ui->listWidget_3->clear();
}

void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;

    if(tabIndex == 0)
        request.makeFeaturedRequest();
    else if(tabIndex == 1)
        request.makeTopRequest();
    else if(tabIndex == 2)
    {
        if(!settings.value("username").isNull())
            request.makeFollowRequest(settings.value("username").toString());
    }
}

void MainWindow::timedDatabaseRead()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;
    streamDataList = request.timedDatabaseRead(tabIndex);
    if(!streamDataList.isEmpty())
        this->addItemToListView(tabIndex,streamDataList);
}

void MainWindow::on_actionAdd_User_triggered()
{
    this->usernameDialog("new");
}

void MainWindow::on_actionExit_triggered()
{
    this->~MainWindow();
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
            QString displayName = streamData[0].replace(" ","");
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
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            if(!displayName.isEmpty())
                ui->listWidget_3->addItem(stream);
        }
        ui->listWidget_3->sortItems();
    }
    else if(index == 3)
    {
        ui->listWidget_4->clear();
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget_4->addItem(stream);
        }
    }
}

void MainWindow::changeStatusBar()
{
    if(request.checkConnection())
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}

void MainWindow::searchTabRequest()
{
    QString search = ui->lineEdit->text();
    if(!search.isEmpty())
        request.makeSearchRequest(search);
}

void MainWindow::on_pushButton_pressed()
{
    this->searchTabRequest();
}

void MainWindow::on_lineEdit_returnPressed()
{
    this->searchTabRequest();
}
