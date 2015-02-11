#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    this->createSignalSlots();
    this->changeStatusBar();
    this->styleItems();
}
MainWindow::~MainWindow()
{
    delete ui;
}

//Adds items to the list view
void MainWindow::addItemToListView(int index, QList<QStringList> streams)
{
    if(index == 0)
    {
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            QByteArray imageArray = request.readStreamImage(displayName);
            QPixmap image;
            if(image.loadFromData(imageArray,0))
            {
                QListWidgetItem *item = new QListWidgetItem(QPixmap(image), stream,ui->listWidget);
                item->setSizeHint(QSize(item->sizeHint().width(),20));
                request.changeDisplayVariable("featured", displayName);
            }
        }
        ui->listWidget->sortItems();
    }
    else if(index == 1)
    {
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            request.changeDisplayVariable("followed", displayName);
            if(!displayName.isEmpty())
                ui->listWidget_2->addItem(stream);
        }
        ui->listWidget_2->sortItems();
    }
    else if(index == 2)
    {
        foreach (QStringList streamData, streams)
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            request.changeDisplayVariable("search", displayName);
            ui->listWidget_3->addItem(stream);
        }
    }
}

//Changes status bar at the bottom for internet connection.
void MainWindow::changeStatusBar()
{
    if(request.checkConnection())
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}

//Creates signals and slots for the mainwindow.
void MainWindow::createSignalSlots()
{
    connect((&request),SIGNAL(usernameDialogSignal(QString)),this,SLOT(usernameDialog(QString)));
    connect((&request),SIGNAL(clearFollowList()),this,SLOT(followListClear()));
    connect((&timerManager),SIGNAL(requestData()),this,SLOT(timedDataRequest()));
    connect((&timerManager),SIGNAL(readDatabase()),this,SLOT(timedDatabaseRead()));
    connect((&timerManager),SIGNAL(checkConnection()),this,SLOT(changeStatusBar()));
    connect((&timerManager),SIGNAL(imageRequest()),this,SLOT(timedImageRequest()));
    connect((&timerManager),SIGNAL(removeOfflineStreams()),this,SLOT(timedOfflineRemoval()));
    connect((ui->listWidget),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_2),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_3),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
}

//Slot to clear the follow list (this is used when username is changed)
void MainWindow::followListClear()
{
    ui->listWidget_3->clear();
}

void MainWindow::onListItemDoubleClicked(QListWidgetItem *item)
{
    QStringList username = item->text().split(":");
    QString url = "http://www.twitch.tv/" + username[0];
    QDesktopServices::openUrl(url);
}

//Adds a new username to QSettings
void MainWindow::on_actionAdd_User_triggered()
{
    this->usernameDialog("new");
}

//Exit
void MainWindow::on_actionExit_triggered()
{
    this->~MainWindow();
}

//Enter pressed on the search window
void MainWindow::on_lineEdit_returnPressed()
{
    this->searchTabRequest();
}

//Submit button
void MainWindow::on_pushButton_pressed()
{
    this->searchTabRequest();
}

//Tab switched signal
void MainWindow::on_tabWidget_currentChanged()
{
    request.clearObjectName();
    this->timedDataRequest();
    this->timedDatabaseRead();
}

//Requests search string when submitted (through enter or clicking the button)
void MainWindow::searchTabRequest()
{
    ui->listWidget_3->clear();
    QString search = ui->lineEdit->text();
    if(!search.isEmpty())
        request.makeSearchRequest(search);
}

void MainWindow::styleItems()
{
    QPalette p = ui->listWidget->palette();
    QPalette p2 = ui->listWidget_2->palette();
    QPalette p3 = ui->listWidget_3->palette();
    p.setColor(QPalette::Highlight, QColor(153,126,175));
    ui->listWidget->setPalette(p);
    ui->listWidget_2->setPalette(p);
    ui->listWidget_3->setPalette(p);
}

//Reads the database for the current tab on a timer.
void MainWindow::timedDatabaseRead()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;
    streamDataList = request.timedDatabaseRead(tabIndex);
    if(!streamDataList.isEmpty())
        this->addItemToListView(tabIndex,streamDataList);
}

//Requests the data for the current open tab on a timer.
void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();

    if(tabIndex == 0)
        request.makeFeaturedRequest();
    else if(tabIndex == 1)
    {
        if(!settings.value("username").isNull())
            request.makeFollowRequest(settings.value("username").toString());
    }
}

//Reads the database for the current tab on a timer.
void MainWindow::timedImageRequest()
{
    QList<QStringList> streams = request.getStreamListNoImage();
    if(!streams.isEmpty())
        request.makeImageRequest(streams[0]);
}

//Reads the database for the current tab on a timer.
void MainWindow::timedOfflineRemoval()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QStringList streams = request.timedOfflineRemoval(tabIndex);
    if(!streams.isEmpty())
        if(tabIndex == 0)
            for(int i = 0; i < ui->listWidget->count(); ++i)
                qDebug() << ui->listWidget->item(i)->text();
}

//Slot for the username dialog, will repop the dialog box if the username isn't found.
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
