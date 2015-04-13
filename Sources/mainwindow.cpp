#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->configureGUI();
}

MainWindow::~MainWindow()
{
    QApplication::quit();
}

//Adds items to the list view when database fires the additem signal.  This is fired
//when a new item is inserted into the database.
void MainWindow::addItemToListView(QStringList streamData)
{
    if(!streamData.isEmpty())
    {
        QString username = streamData[0].replace(" ","");
        QString game = streamData[1];
        QString viewers = streamData[2];
        QString stream = username + ": (" + viewers + ") " + game;
        if(stream.contains("Notfound"))
            stream = "Not Found or Offline";
        QString requestType = streamData[3];
        QString logo = streamData[4];
        if(requestType == "featured")
        {
            if(!username.isEmpty())
                ui->listWidget->addItem(stream);
            ui->listWidget->sortItems();
        }
        else if(requestType == "followed")
        {
            if(!username.isEmpty())
            {
                ui->listWidget_2->addItem(stream);
                QString notificationSetting = request.getSettingsValue("notification");
                if(notificationSetting == "false" || notificationSetting.isNull())
                {
                    QString message = username + " is online!\nPlaying: " + game;
                    sysTray.showMessage("Following", message,QSystemTrayIcon::Critical);
                }
            }
            ui->listWidget_2->sortItems();
        }
        else if(requestType == "search")
        {
            if(!username.isEmpty())
                ui->listWidget_3->addItem(stream);
            ui->listWidget_3->sortItems();
        }
        this->updateItemIndex(requestType);
    }
}

//Changes status bar at the bottom for internet connection.
void MainWindow::changeStatusBar()
{
    QString networkStatus;

    if(request.checkConnection())
        networkStatus = "Online";
    else
        networkStatus = "Offline";

    QString status = "Network: " + networkStatus;
    statusBar()->showMessage(status);
}

//Clears the list views (featured, followed and search)
void MainWindow::clearListViews()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
}

//Configure look and feel of the GUI
void MainWindow::configureGUI()
{
    ui->tabWidget->setCurrentIndex(0);
    QString icon = QCoreApplication::applicationDirPath() + "/icon.png";
    this->createSignalSlots();
    this->changeStatusBar();
    this->styleItems();
    this->enableMouseTracking();
    sysTray.setIcon(QIcon(icon));
    sysTray.show();
}

//Creates signals and slots for the mainwindow.
void MainWindow::createSignalSlots()
{
    connect((&db),SIGNAL(addStreamToView(QStringList)),this,SLOT(addItemToListView(QStringList)));
    connect((&db),SIGNAL(deleteStreamerFromListView(int)),this,SLOT(removeOfflineStreamer(int)));
    connect((&db),SIGNAL(listViewClears()),this,SLOT(clearListViews()));
    connect((&db),SIGNAL(updateStreamInView(QStringList,int)),this,SLOT(updateItemInListView(QStringList,int)));
    connect((&request),SIGNAL(usernameDialogSignal(QString)),this,SLOT(usernameDialog(QString)));
    connect((&request),SIGNAL(clearFollowList()),this,SLOT(followListClear()));
    connect((&request),SIGNAL(manageOnlineStreamers(QString)),&db,SLOT(manageOnlineStreamers(QString)));
    connect((&request),SIGNAL(deleteOfflineStreamersFromDB()),&db,SLOT(removeOfflineStreamers()));
    connect((&request),SIGNAL(storeIndex(QString,QString,int)),&db,SLOT(storeItemIndex(QString,QString,int)));
    connect((&request),SIGNAL(storeStreamData(QStringList, QString)),&db,SLOT(storeStreamData(QStringList,QString)));
    connect((&request),SIGNAL(truncateStreamData()),&db,SLOT(truncateStreamData()));
    connect((&request),SIGNAL(truncateSearchData()),&db,SLOT(truncateSearchData()));
    connect((&timerManager),SIGNAL(backgroundRequest()),this,SLOT(timedBackgroundRequest()));
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
        if(username[0] != "No Username Set" || username[0] != "Not Found or Offline")
        {
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
    if(username[0] != "No Username Set" && username[0] != "Not Found or Offline")
    {
        QString url = "http://www.twitch.tv/" + username[0];
        QDesktopServices::openUrl(url);
    }
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

//Open options dialog when options are selected
void MainWindow::on_Options_triggered()
{
    Dialog *d = new Dialog();
    d->show();
}

//Submit button
void MainWindow::on_pushButton_pressed()
{
    this->searchTabRequest();
    ui->listWidget_3->clear();
}

//Tab switched signal
void MainWindow::on_tabWidget_currentChanged()
{
    this->timedDataRequest();
}


//Quit application, used to be on_Exit, but osx likes Quit better (implements cmd-q natively)
void MainWindow::on_Quit_triggered()
{
    this->close();
}

void MainWindow::removeOfflineStreamer(int itemIndex)
{
    delete ui->listWidget_2->item(itemIndex);
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

//This request will make the proper calls to get data for tabs that aren't currently opened.
//Having this timer will quicken data access for the user.
void MainWindow::timedBackgroundRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();

    if(tabIndex == 0)
    {
        QString username = request.getSettingsValue("username");
        if(!username.isEmpty())
            request.makeFollowRequest(username);
        else
        {
            ui->listWidget_2->clear();
            ui->listWidget_2->addItem("No Username Set");
        }
    }
    else if(tabIndex == 1)
        request.makeFeaturedRequest();
    else
    {
        request.makeFeaturedRequest();
        QString username = request.getSettingsValue("username");
        if(!username.isEmpty())
            request.makeFollowRequest(username);
        else
        {
            ui->listWidget_2->clear();
            ui->listWidget_2->addItem("No Username Set");
        }
    }
}

//Requests the data for the current open tab on a timer.
void MainWindow::timedDataRequest()
{
    int tabIndex = ui->tabWidget->currentIndex();

    if(tabIndex == 0)
        request.makeFeaturedRequest();
    else if(tabIndex == 1)
    {
        QString username = request.getSettingsValue("username");
        if(!username.isEmpty())
            request.makeFollowRequest(username);
        else
        {
            ui->listWidget_2->clear();
            ui->listWidget_2->addItem("No Username Set");
        }
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
        QString displayName = streamData[0].replace(" ","");
        QString game = streamData[1];
        QString viewers = streamData[2];
        QString stream = displayName + ": (" + viewers + ") " + game;
        QString requestType = streamData[3];
        if(requestType == "featured")
            ui->listWidget->item(index)->setText(stream);
        else if(requestType == "featured")
            ui->listWidget_2->item(index)->setText(stream);
    }
}

//Updates the item index in the database.  This is needed because sorting the listview rearranges the
//indexes so they need to be updated manually after every sort.
void MainWindow::updateItemIndex(QString requestType)
{
    if(requestType == "featured")
    {
        int count = ui->listWidget->count();
        for(int itemCount = 0; itemCount < count; itemCount++)
        {
            QString item = ui->listWidget->item(itemCount)->text();
            QStringList stream = item.split(":");
            request.storeItemIndex(requestType,stream[0],itemCount);
        }
    }
    else if(requestType == "followed")
    {
        int count = ui->listWidget_2->count();
        for(int itemCount = 0; itemCount < count; itemCount++)
        {
            QString item = ui->listWidget_2->item(itemCount)->text();
            QStringList stream = item.split(":");
            request.storeItemIndex(requestType,stream[0],itemCount);
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
