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
#include "json.h"
#include "networkoperations.h"
#include "database.h"
#include "timers.h"
#include "requesthandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QList<QStringList> followsList;
    ~MainWindow();
    void addItemToListView(int,QList<QStringList>);

public slots:
    void followListClear();
    void usernameDialog(QString);

private slots:
    void changeStatusBar();
    void displayToolTip(QListWidgetItem* item);
    void onListItemDoubleClicked(QListWidgetItem* item);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_lineEdit_returnPressed();
    void on_pushButton_pressed();
    void on_tabWidget_currentChanged();
    void timedDataRequest();
    void timedDatabaseRead();
    void timedNetworkRequest();

private:
    Ui::MainWindow *ui;
    requestHandler request;
    timers timerManager;
    QSettings settings;
    void createSignalSlots();
    void enableMouseTracking();
    void searchTabRequest();
    void styleItems();
};

#endif // MAINWINDOW_H
