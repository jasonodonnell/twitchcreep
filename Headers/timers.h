#ifndef TIMERS_H
#define TIMERS_H

#include <QObject>
#include <QTimer>

class timers : public QObject
{
    Q_OBJECT
public:
    explicit timers(QObject *parent = 0);
    ~timers();

signals:
    void requestData();
    void readDatabase();
    void checkConnection();
public slots:

private slots:
    void dataRequest();
    void databaseRead();
    void networkConnection();
private:
    QTimer *requestTimer = new QTimer(this);
    QTimer *readTimer = new QTimer(this);
    QTimer *connectionTimer = new QTimer(this);
    void createTimerSignals();
    void startTimers();
};

#endif // TIMERS_H