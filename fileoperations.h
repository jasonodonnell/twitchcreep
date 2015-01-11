#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include <QStringList>
#include <QFile>
#include <QObject>

class fileOperations : public QObject
{
    Q_OBJECT
public:
    explicit fileOperations(QObject *parent = 0);
    void writeStreamersToDisk(QStringList);
    ~fileOperations();

signals:

public slots:
private:

};

#endif // FILEOPERATIONS_H
