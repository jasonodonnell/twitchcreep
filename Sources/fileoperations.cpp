#include "fileoperations.h"
#include <QFile>
#include <QDebug>
#include <QDir>

fileOperations::fileOperations(QObject *parent) : QObject(parent)
{

}

void fileOperations::writeStreamersToDisk(QStringList streamers)
{
    QFile streamFile("/Users/Wizard/streamers.dat");
    streamFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream fileIO(&streamFile);
    if (streamFile.exists())
    {
        foreach(QString str, streamers)
        {
           fileIO << str << endl;
        }
        streamFile.close();
    }
}

void fileOperations::readStreamersFromDisk()
{
    QFile streamFile("/Users/Wizard/streamers.dat");
    streamFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream fileIO(&streamFile);
    if (streamFile.exists())
    {
        while (!fileIO.atEnd())
        {
            QString line = fileIO.readLine();
            qDebug() << line;
        }
        streamFile.close();
    }
}

fileOperations::~fileOperations()
{

}

