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
    this->enableMouseTracking();
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
            if(!displayName.isEmpty())
                ui->listWidget->addItem(stream);
            request.changeDisplayVariable("featured", displayName);
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
    connect((&timerManager),SIGNAL(networkRequest()),this,SLOT(timedNetworkRequest()));
    connect((ui->listWidget),SIGNAL(itemEntered(QListWidgetItem*)),this,SLOT(displayToolTip(QListWidgetItem*)));
    connect((ui->listWidget),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_2),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
    connect((ui->listWidget_3),SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListItemDoubleClicked(QListWidgetItem*)));
}

void MainWindow::displayToolTip(QListWidgetItem *item)
{
    QStringList username = item->text().split(":");
    QString status = request.getStatus(username[0]);
    QWidget::setToolTip(status);
}

//Enables mouse tracking. this is required to show tooltips.
void MainWindow::enableMouseTracking()
{
    ui->listWidget->setMouseTracking(true);
    ui->listWidget_2->setMouseTracking(true);
    ui->listWidget_3->setMouseTracking(true);
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

void MainWindow::timedNetworkRequest()
{
    request.makeRequest();
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
