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
#include <QMenu>
#include "dialog.h"
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
    void clearSearchView();
    void displayToolTip(QListWidgetItem* item);
    void onListItemDoubleClicked(QListWidgetItem* item);
    void on_actionAdd_User_triggered();
    void on_actionClear_User_triggered();
    void on_Quit_triggered();
    void on_lineEdit_returnPressed();
    void on_Options_triggered();
    void on_pushButton_pressed();
    void on_systray_clicked(QSystemTrayIcon::ActivationReason clicked);
    void on_tabWidget_currentChanged();
    void removeOfflineStreamer(int itemIndex);
    void timedBackgroundRequest();
    void timedDataRequest();
    void timedNetworkRequest();
    void updateItemInListView(QStringList streamData, int index);
private:
    database db;
    timers timerManager;
    Ui::MainWindow *ui;
    QSystemTrayIcon sysTray;
    void createSignalSlots();
    void configureGUI();
    void enableMouseTracking();
    void searchTabRequest();
    void styleItems();
    void updateItemIndex(QString requestType);
    void showContextMenu();
};

#endif // MAINWINDOW_H
