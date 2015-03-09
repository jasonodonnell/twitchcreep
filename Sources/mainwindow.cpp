#include "../Headers/mainwindow.h"
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
    this->enableMouseTracking();
}
MainWindow::~MainWindow()
{
    delete ui;
}

//Adds items to the list view when database fires the additem signal.  This is fired
//when a new item is inserted into the database.
void MainWindow::addItemToListView(QStringList streamData)
{
    if(!streamData.isEmpty())
    {
        if(streamData[3] == "featured")
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            if(!displayName.isEmpty())
                ui->listWidget->addItem(stream);
            ui->listWidget->sortItems();
            this->updateItemIndex(streamData[3]);
        }
        else if(streamData[3] == "followed")
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            if(!displayName.isEmpty())
                ui->listWidget_2->addItem(stream);
            ui->listWidget_2->sortItems();
            this->updateItemIndex(streamData[3]);
        }
        else if(streamData[3] == "search")
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            if(!displayName.isEmpty())
                ui->listWidget_3->addItem(stream);
            ui->listWidget_3->sortItems();
        }
    }
}

//Changes status bar at the bottom for internet connection.
void MainWindow::changeStatusBar()
{
    QString networkStatus;
    QString username = request.getSettingsValue("username");

    if(request.checkConnection())
        networkStatus = "Online";
    else
        networkStatus = "Offline";

    if(username.isNull())
        username = "Not set";

    QString status = "Network: " + networkStatus;
    statusBar()->showMessage(status);

}

void MainWindow::clearListViews()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
}

//Creates signals and slots for the mainwindow.
void MainWindow::createSignalSlots()
{
    connect((&db),SIGNAL(addStreamToView(QStringList)),this,SLOT(addItemToListView(QStringList)));
    connect((&db),SIGNAL(listViewClears()),this,SLOT(clearListViews()));
    connect((&db),SIGNAL(updateStreamInView(QStringList,int)),this,SLOT(updateItemInListView(QStringList,int)));
    connect((&request),SIGNAL(usernameDialogSignal(QString)),this,SLOT(usernameDialog(QString)));
    connect((&request),SIGNAL(clearFollowList()),this,SLOT(followListClear()));
    connect((&request),SIGNAL(manageOnlineStreamers(QString)),&db,SLOT(manageOnlineStreamers(QString)));
    connect((&request),SIGNAL(storeIndex(QString,QString,int)),&db,SLOT(storeItemIndex(QString,QString,int)));
    connect((&request),SIGNAL(storeStreamData(QStringList, QString)),&db,SLOT(storeStreamData(QStringList,QString)));
    connect((&request),SIGNAL(truncateStreamData()),&db,SLOT(truncateStreamData()));
    connect((&timerManager),SIGNAL(checkConnection()),this,SLOT(changeStatusBar()));
    connect((&timerManager),SIGNAL(networkRequest()),this,SLOT(timedNetworkRequest()));
    connect((&timerManager),SIGNAL(requestData()),this,SLOT(timedDataRequest()));
    connect((ui->listWidget),SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(displayToolTip(QListWidgetItem*)));
    connect((ui->listWidget_2),SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(displayToolTip(QListWidgetItem*)));
    connect((ui->listWidget_3),SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(displayToolTip(QListWidgetItem*)));
    connect((ui->listWidget),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_2),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_3),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
}

//Slot to display a tooltip for the currently hovered item.
void MainWindow::displayToolTip(QListWidgetItem *item)
{
    if(item->isSelected())
    {
        QStringList username = item->text().split(":");
        int tabIndex = ui->tabWidget->currentIndex();
        QString requestType;
        if(tabIndex == 0)
            requestType = "featured";
        else if(tabIndex == 1)
            requestType = "followed";
        else if(tabIndex == 2)
            requestType = "search";
        QString status = db.retrieveStatus(username[0], requestType);
        QWidget::setToolTip(status);
    }
    else
        QWidget::setToolTip("");
}

//Enables mouse tracking. this is required to show tooltips.
void MainWindow::enableMouseTracking()
{
    ui->listWidget->setMouseTracking(true);
    ui->listWidget_2->setMouseTracking(true);
    ui->listWidget_3->setMouseTracking(true);
    QWidget::setToolTipDuration(3000);
}

//Slot to clear the follow list (this is used when username is changed)
void MainWindow::followListClear()
{
    ui->listWidget_3->clear();
}

//Launch browser with stream url on a double clicked item.
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

//Clears username out of qsettings
void MainWindow::on_actionClear_User_triggered()
{
    request.setSettingsValue("username","");
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
    this->timedDataRequest();
}

//Exit app when close is clicked.
void MainWindow::on_actionExit_triggered()
{
    this->~MainWindow();
}

//Requests search string when submitted (through enter or clicking the button)
void MainWindow::searchTabRequest()
{
    ui->listWidget_3->clear();
    QString search = ui->lineEdit->text();
    if(!search.isEmpty())
        request.makeSearchRequest(search);
}

//Apply styles to various views.
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

//Requests the data for the current open tab on a timer.
void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();
    QString username = request.getSettingsValue("username");

    if(tabIndex == 0)
        request.makeFeaturedRequest();
    else if(tabIndex == 1)
    {
        if(!username.isNull())
            request.makeFollowRequest(username);
    }
}

//Make a network request on a timer.  This is based on current tab index.
void MainWindow::timedNetworkRequest()
{
    request.makeRequest();
}

//Update items in the list view.  When the database updates an entry in the db, it'll fire
//a signal to activate this update.
void MainWindow::updateItemInListView(QStringList streamData,int index)
{
    if(!streamData.isEmpty())
    {
        if(streamData[3] == "featured")
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget->item(index)->setText(stream);
            ui->listWidget->sortItems();
        }
        else if(streamData[3] == "followed")
        {
            QString displayName = streamData[0].replace(" ","");
            QString game = streamData[1];
            QString viewers = streamData[2];
            QString stream = displayName + ": (" + viewers + ") " + game;
            ui->listWidget_2->item(index)->setText(stream);
            //ui->listWidget_2->sortItems();
        }
    }
}

void MainWindow::updateItemIndex(QString requestType)
{
    if(requestType == "featured")
    {
        int count = ui->listWidget->count();
        for(int i = 0; i < count; i++)
        {
            QString item = ui->listWidget->item(i)->text();
            QStringList stream = item.split(":");
            request.storeItemIndex(requestType,stream[0],i);
        }
    }
    else if(requestType == "followed")
    {
        int count = ui->listWidget_2->count();
        for(int i = 0; i < count; i++)
        {
            QString item = ui->listWidget_2->item(i)->text();
            QStringList stream = item.split(":");
            request.storeItemIndex(requestType,stream[0],i);
        }
    }
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
