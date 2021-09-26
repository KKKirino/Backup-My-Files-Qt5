#include "job.h"

Job::Job(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QList<QString>>("QList<QString>");
}

void Job::startJob(int type,
                   QList<QString> packPaths,
                   QString path,
                   QString destPath)
{
    qDebug("starting job");
    switch (type) {
    case JobType::PACK:
        pack(packPaths, path, destPath);
            break;
    case JobType::UNPACK:
        unpack(packPaths, path, destPath);
            break;
//    case JobType::COMPRESS:
//        compress(info);
//        break;
//    case JobType::DECOMPRESS:
//        decompress(info);
//        break;
//    case JobType::ENCRYPT:
//        encrypt(info);
//        break;
//    case JobType::DECTYPRT:
//        decrypt(info);
//        break;
    case JobType::BACKUP:
        backup(packPaths, "", destPath);
        break;
    default:
        emit jobFinished("未知任务");
    }
}

void Job::pack(QList<QString> packPaths,
               QString path,
               QString destPath,
               bool isBackUp) {
    qDebug("start pack");
    if (packPaths.count() == 0) return emit jobFinished("请指定要打包的文件");

    auto destFolder = QDir(destPath);
    if (!destFolder.exists()) return emit jobFinished("目标路径不存在");

    QList<QFileInfo> _packPaths;
    for (auto p : packPaths) {
        _packPaths.append(QFileInfo(p));
    }

    QString desFileName = _packPaths.first().fileName();
    if (packPaths.count() > 1) desFileName += QString("等 %1 个文件").arg(packPaths.count() - 1);
    desFileName += ".pack";

    auto destFilePath = destFolder.filePath(desFileName);
    QFile destFile(destFilePath);

    try {
        destFile.open(QFile::WriteOnly);

        // 遍历所有文件，读取信息
        int totalCount = 0;
        long long totalSize = 0;
        QList<QFileInfo> allPaths;
        QList<QFileInfo> stack(_packPaths);
        while (stack.count())
        {
            auto current = stack.last();
            stack.pop_back();
            // 只处理存在的文件，不处理文件夹
            if (!current.exists()) continue;
            // 过滤空文件
            if (current.isFile() && current.isReadable() && current.size() > 0)
            {
                totalCount += 1;
                totalSize += current.size();
                allPaths.append(current);
            }
            else if (current.isDir())
            {
                auto fileLists = QDir(current.filePath()).entryInfoList(QDir::NoFilter, QDir::DirsFirst);
                for (auto& p : fileLists) {
                    if (p.fileName().startsWith('.') && p.fileName().endsWith('.')) continue;
                    stack.append(p);
                };
            }
        }

        qDebug("total %d files with size of %lld", totalCount, totalSize);


        QDataStream out(&destFile);

        // 写入类型信息，以及文件数量
        out << packMagic << totalCount;

        // 写入备份根路径
        auto basePath = QFileInfo(packPaths[0]).path().toUtf8();
        qDebug("base path %d", basePath.size());
        qDebug(basePath);

        out << basePath.size() << basePath;

        // 计算元信息，依次写入字符串大小和字符串内容
        for (auto p : allPaths)
        {
            auto name = p.absoluteFilePath().toUtf8();
            int size = name.size();
            out << size << name;
//            qDebug("size: %d", size);
//            qDebug(name);
        }

        for (int i = 0; i < totalCount; i += 1)
        {
            QFile current(allPaths[i].absoluteFilePath());
            emit jobUpdated(i, totalCount);
            // 将所有文件写入目标文件
            current.open(QFile::ReadOnly);
            auto content = current.readAll();
            out << content.size() << content;
//            qDebug("write size: %d", content.size());
//            qDebug(allPaths[i].absoluteFilePath().toUtf8());
        }

        out.commitTransaction();
        destFile.flush();
        destFile.close();
        emit jobFinished(QString("已打包到：%1").arg(QDir(destFilePath).absolutePath()));

        if (isBackUp)
        {
            unpack({}, destFilePath, destPath, true);
        }
    } catch (errno_t e) {
        return emit jobFinished("打包文件出错");
    }
}

void Job::unpack(QList<QString> packPaths,
                 QString path,
                 QString destPath,
                 bool shouldRemoveSource) {
    qDebug("start unpack");

    QFile packFile(path);
    if (!packFile.exists()) return emit jobFinished("解包文件不存在");
    packFile.open(QFile::ReadOnly);

    // 开始读取文件
    QDataStream source(&packFile);

    quint32 magic = 0;
    source >> magic;
    qDebug("magic %d", magic);
    if (magic != packMagic) return emit jobFinished("无效的打包文件");

    int size = 0;

    int fileCount = 0;
    source >> fileCount;

    qDebug("read files: %d files", fileCount);

    // 读取根目录信息
    source >> size;
    size = 24;
    char* rawBasePath = new char[size]();
    source >> rawBasePath;
    QDir basePath(rawBasePath);
    qDebug("read base path %d", size);
    qDebug(rawBasePath);
    delete [] rawBasePath;

    QList<QString> allPaths;
    for (int i = 0; i < fileCount; i += 1)
    {
        source >> size;
        char* name = new char[size]();
        source >> name;
//        qDebug("read: %d", size);
//        qDebug(name);
        allPaths.append(name);
        delete [] name;
    }

    qDebug("dest path");
    qDebug(destPath.toUtf8());
    QDir destFolder = QDir(destPath);

    emit jobUpdated(0, fileCount);

    // 读取文件内容
    for (int i = 0; i < fileCount; i += 1)
    {
        source >> size;
        char* content = new char[size]();
        source >> content;
//        qDebug("read file: %d", size);
        // 根据相对路径，写入目录结构
        QString relativePath = basePath.relativeFilePath(allPaths[i]);
//        qDebug("relative");
//        qDebug(relativePath.toUtf8());
        QString destFilePath = destFolder.absoluteFilePath(relativePath);

        // 如果路径不存在，则创建对应的路径
        QDir destFileDir = QFileInfo(destFilePath).dir();
        destFileDir.mkpath(destFileDir.absolutePath());
//        qDebug("%d", destFileDir.exists());
//        qDebug(destFileDir.absolutePath().toUtf8());
        // 写入文件内容
        QFile destOutput(destFilePath);
        destOutput.open(QFile::WriteOnly);
        destOutput.write(content);
        destOutput.flush();
        destOutput.close();
        delete [] content;

        emit jobUpdated(i, fileCount);
    }

    // 如果是备份，则需要删除源文件
    if (shouldRemoveSource) {
        packFile.remove();
        emit jobFinished(QString("已备份到：%1").arg(destPath));
    } else {
        emit jobFinished(QString("已还原到：%1").arg(destPath));
    }
}
//void Job::compress(JobInfo info) {};
//void Job::decompress(JobInfo info) {};
//void Job::encrypt(JobInfo info) {};
//void Job::decrypt(JobInfo info) {};
void Job::backup(QList<QString> packPaths,
                 QString path,
                 QString destPath) {
    pack(packPaths, path, destPath, true);
};
