#include "disk_file.h"

DiskFile::DiskFile(QString path):
    path(path)
{
    dir = QDir(path);

    name = dir.path();
    isFolder = dir.isReadable();
}


QFileInfoList DiskFile::getChildren() {
    return dir.entryInfoList();
}
