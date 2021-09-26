#ifndef DISK_FILE_H
#define DISK_FILE_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>

class DiskFile
{
public:
    QString path;
    QString name;
    QDir dir;
    bool isFolder;

    DiskFile(QString path);
    QFileInfoList getChildren();
};

#endif // DISK_FILE_H
