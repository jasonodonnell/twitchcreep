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
    void displayToolTip(QListWidgetItem* item);
    void onListItemDoubleClicked(QListWidgetItem* item);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_lineEdit_returnPressed();
    void on_pushButton_pressed();
    void on_tabWidget_currentChanged();
    void timedDataRequest();
    void timedNetworkRequest();
    void updateItemInListView(QStringList streamData, int index);

    void on_actionClear_User_triggered();

    void clearListViews();
private:
    database db;
    timers timerManager;
    Ui::MainWindow *ui;
    void createSignalSlots();
    void enableMouseTracking();
    void searchTabRequest();
    void styleItems();
};

#endif // MAINWINDOW_H
