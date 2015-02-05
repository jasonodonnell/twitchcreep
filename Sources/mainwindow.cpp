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
}
MainWindow::~MainWindow()
{
    delete ui;
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

//Slot to clear the follow list (this is used when username is changed)
void MainWindow::followListClear()
{
    ui->listWidget_3->clear();
}


//Requests the data for the current open tab on a timer.
void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();

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

//Reads the database for the current tab on a timer.
void MainWindow::timedDatabaseRead()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QList<QStringList> streamDataList;
    streamDataList = request.timedDatabaseRead(tabIndex);
    if(!streamDataList.isEmpty())
        this->addItemToListView(tabIndex,streamDataList);
}

//Reads the database for the current tab on a timer.
void MainWindow::timedImageRequest()
{
    QList<QStringList> streams = request.getStreamListNoImage();
    if(!streams.isEmpty())
        request.makeImageRequest(streams[0]);
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

//Tab switched signal
void MainWindow::on_tabWidget_currentChanged()
{
    request.clearObjectName();
    this->timedDataRequest();
    this->timedDatabaseRead();
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
            QByteArray imageArray = request.readStreamImage(displayName);
            QPixmap image;
            if(image.loadFromData(imageArray,"PNG"))
                QListWidgetItem *item = new QListWidgetItem(QPixmap(image), stream,ui->listWidget);
            else if(image.loadFromData(imageArray,"JPEG"))
                QListWidgetItem *item = new QListWidgetItem(QPixmap(image), stream,ui->listWidget);
            else
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

//Changes status bar at the bottom for internet connection.
void MainWindow::changeStatusBar()
{
    if(request.checkConnection())
        statusBar()->showMessage(tr("Status: Online"));
    else
        statusBar()->showMessage(tr("Status: Offline"));
}

//Requests search string when submitted (through enter or clicking the button)
void MainWindow::searchTabRequest()
{
    QString search = ui->lineEdit->text();
    if(!search.isEmpty())
        request.makeSearchRequest(search);
}

//Submit button
void MainWindow::on_pushButton_pressed()
{
    this->searchTabRequest();
}

//Enter pressed on the search window
void MainWindow::on_lineEdit_returnPressed()
{
    this->searchTabRequest();
}
