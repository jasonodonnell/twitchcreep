#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json.h"
#include "imageoperations.h"
#include "networkoperations.h"
#include <QDebug>
#include <QListView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeStatusBar();
    void addItemToListView(int,QList<QStringList>);
    void tabRequest(int);
private slots:
    void requestReady(QByteArray,QString);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int);

private:
    Ui::MainWindow *ui;
    json jsonParser;
    imageOperations image;
    networkOperations networking;

};

#endif // MAINWINDOW_H
