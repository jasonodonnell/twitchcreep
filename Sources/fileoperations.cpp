#include "fileoperations.h"
#include <QFile>
#include <QDebug>
#include <QDir>

fileOperations::fileOperations(QObject *parent) : QObject(parent)
{
}

void fileOperations::writeStreamersToDisk(QStringList streamers)
{
    QFile file("/Users/Wizard/streamers.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream fileStream(&file);
    if (file.exists())
    {
        foreach(QString str, streamers)
        {
            fileStream << str << endl;

        }
        file.close();
    }
}

fileOperations::~fileOperations()
{

}

