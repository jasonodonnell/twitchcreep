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
    void getFeaturedStreams(QByteArray data);
    ~json();

signals:

public slots:

private:
};

#endif // JSON_H
