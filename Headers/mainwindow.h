#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QListView>
#include <QInputDialog>
#include <QSettings>
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

    void changeStatusBar();
    void addItemToListView(int,QList<QStringList>);
    //void tabRequest(int);
public slots:
    void usernameDialog(QString);
    void followListClear();

private slots:
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_tabWidget_currentChanged();
    void timedDataRequest();
    void timedDatabaseRead();
    void on_pushButton_pressed();
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    requestHandler request;
    QSettings settings;
    timers timerManager;
    void createSignalSlots();
    void searchTabRequest();
};

#endif // MAINWINDOW_H
