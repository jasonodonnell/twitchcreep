#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "json.h"
#include "fileoperations.h"

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
    void streamersReady(QByteArray data);

private:
    Ui::MainWindow *ui;
    json streamsFollowed;
    fileOperations streamerFile;

};

#endif // MAINWINDOW_H
