#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json.h"
#include "imageoperations.h"
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

    void changeStatusBar();
private slots:
    void requestReady(QByteArray data);
    void on_actionAdd_User_triggered();
    void on_actionExit_triggered();
    void on_pushButton_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    json jsonParser;
    imageOperations image;
    networkOperations networking;

};

#endif // MAINWINDOW_H
