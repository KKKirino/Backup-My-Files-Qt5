#ifndef JOBITEM_H
#define JOBITEM_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QUrl>
#include <QDesktopServices>

#include "job.h"

namespace Ui {
class JobItem;
}

class JobItem : public QWidget
{
    Q_OBJECT

public:
    explicit JobItem(QString name, QString status, int progress, QWidget *parent = nullptr);
    ~JobItem();

    Ui::JobItem *ui;

    QLabel* job_name;
    QLabel* job_status;
    QProgressBar* job_progress;
    QString targetPath;

    QThread thread;
    void start(int type,
               QList<QString> packPaths,
               QString path,
               QString destPath,
               char key = 0);

signals:
    void startJob(int type,
                  QList<QString> packPaths,
                  QString path,
                  QString destPath,
                  char key = 0);

public slots:
    void onJobUpdated(int progress, int total);
    void onJobFinished(QString result);
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // JOBITEM_H
