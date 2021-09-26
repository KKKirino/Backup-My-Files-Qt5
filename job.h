#ifndef JOB_H
#define JOB_H

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QMetaType>
#include <QDataStream>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QObject *parent = nullptr);
    enum JobType {
        PACK,
        UNPACK,
//        COMPRESS,
//        DECOMPRESS,
        ENCRYPT,
        DECTYPRT,
        BACKUP
    };

    quint32 packMagic = 0x12345678;

    struct JobInfo {
        JobType type;
        QList<QString> packPaths;
        QString path; // 加密解密，压缩解压，备份，解包
        QString destPath; // 目标地址
    };
    void pack(
        QList<QString> packPaths,
        QString path,
        QString destPath,
        bool isBackUp = false
    );
    void unpack(QList<QString> packPaths,
                QString path,
                QString destPath,
                bool shouldRemoveSource = false);
//    void compress(JobInfo info);
//    void decompress(JobInfo info);
    void encrypt(QList<QString> packPaths,
                 QString path,
                 QString destPath);
    void decrypt(QList<QString> packPaths,
                 QString path,
                 QString destPath);
    void backup(QList<QString> packPaths,
                QString path,
                QString destPath);

signals:
    void jobFinished(QString result);
    void jobUpdated(int progress, int total);

protected slots:
    void startJob(int type,
                  QList<QString> packPaths,
                  QString path,
                  QString destPath);
};

#endif // JOB_H
