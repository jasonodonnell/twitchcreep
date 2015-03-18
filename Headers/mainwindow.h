    #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QListView>
#include <QInputDialog>
#include <QSettings>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QStringList>
#include <QToolTip>
#include <QSystemTrayIcon>
#include "timers.h"
#include "requesthandler.h"
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    requestHandler request;
    QList<QStringList> followsList;
    ~MainWindow();

public slots:
    void followListClear();
    void usernameDialog(QString);

private slots:
    void addItemToListView(QStringList);
    void changeStatusBar();
    void clearListViews();
    void displayToolTip(QListWidgetItem* item);
    void makeStreamImageRequest(QString requestType, QString logo, QString username);
    void onListItemDoubleClicked(QListWidgetItem* item);
    void on_actionAdd_User_triggered();
    void on_actionClear_User_triggered();
    void on_actionExit_triggered();
    void on_lineEdit_returnPressed();
    void on_pushButton_pressed();
    void on_tabWidget_currentChanged();
    void timedDataRequest();
    void timedNetworkRequest();
    void updateIconImage(QByteArray data, int index, QString requestType);
    void updateItemInListView(QStringList streamData, int index);
    void timedBackgroundRequest();
private:
    database db;
    timers timerManager;
    Ui::MainWindow *ui;
    void createSignalSlots();
    void enableMouseTracking();
    void searchTabRequest();
    void styleItems();
    void updateItemIndex(QString requestType);
    void configureGUI();
};

#endif // MAINWINDOW_H
