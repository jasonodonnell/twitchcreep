#ifndef IMAGEREQUEST_H
#define IMAGEREQUEST_H
#include <QThread>
#include <QObject>
#include <QStringList>

class imageRequest : public QThread
{
    Q_OBJECT
public:
    explicit imageRequest(QObject *parent = 0);
    void run(QList<QStringList> streamsNoImage);
    ~imageRequest();

signals:
    void requestImageSignal(QStringList);

public slots:
};

#endif // IMAGEREQUEST_H
