#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QListView>
#include "json.h"
#include "imageoperations.h"
#include "networkoperations.h"
#include "database.h"

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
    void tabRequest(int);
private slots:
    void requestReady(QByteArray,QString);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_tabWidget_currentChanged(int);

private:
    Ui::MainWindow *ui;
    json jsonParser;
    imageOperations image;
    networkOperations networking;
    database db;

};

#endif // MAINWINDOW_H
