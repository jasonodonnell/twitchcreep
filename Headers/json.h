#ifndef JSONRETRIEVER_H
#define JSONRETRIEVER_H
#include <QObject>

class json : public QObject
{
    Q_OBJECT
public:
    explicit json(QObject *parent = 0);
    QString determineDataSource(QByteArray);
    QStringList getStreamerFollowedList(QByteArray);
    ~json();
    void getStreamData(QByteArray data);
    void getFeaturedStreamData(QByteArray data);
signals:

public slots:

private:
};

#endif // JSON_H
