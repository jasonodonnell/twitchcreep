#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json.h"
#include "fileoperations.h"
#include "networkoperations.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void requestReady(QByteArray data);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    json streamsFollowed;
    fileOperations streamerFile;
    networkOperations networking;

};

#endif // MAINWINDOW_H
