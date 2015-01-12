#ifndef JSONRETRIEVER_H
#define JSONRETRIEVER_H
#include <QObject>

class json : public QObject
{
    Q_OBJECT
public:
    explicit json(QObject *parent = 0);
    QStringList getStreamersList(QByteArray);
    ~json();

signals:

public slots:

private:
<<<<<<< HEAD
=======
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QTimer *timer = new QTimer(this);
>>>>>>> master
};

#endif // JSON_H
