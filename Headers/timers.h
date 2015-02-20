#ifndef TIMERS_H
#define TIMERS_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class timers : public QObject
{
    Q_OBJECT
public:
    explicit timers(QObject *parent = 0);
    ~timers();

signals:
    void checkConnection();
    void networkRequest();
    void readDatabase();
    void removeOfflineStreams();
    void requestData();
    void updateStreams();
public slots:

private slots:
    void dataRequest();
    void databaseRead();
    void displayedOffline();
    void makeNetworkRequest();
    void networkConnection();
    void updateDisplayedStreams();
private:
    QTimer *connectionTimer = new QTimer(this);
    QTimer *displayedOfflineTimer = new QTimer(this);
    QTimer *networkRequestTimer = new QTimer(this);
    QTimer *readTimer = new QTimer(this);
    QTimer *requestTimer = new QTimer(this);
    QTimer *updateStreamsTimer = new QTimer(this);
    void createTimerSignals();
    void startTimers();
};

#endif // TIMERS_H
