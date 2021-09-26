#include "job_item.h"
#include "ui_job_item.h"

JobItem::JobItem(QString name, QString status, int progress, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JobItem)
{
    ui->setupUi(this);
    setObjectName("job_item");

    job_name = this->findChild<QLabel*>("job_name");
    job_status = this->findChild<QLabel*>("job_status");
    job_progress = this->findChild<QProgressBar*>("job_progress");

    job_name->setText(name);
    job_status->setText(status);
    job_progress->setValue(progress);

    qRegisterMetaType<QList<QString>>("QList<QString>");

    // config job details
    auto job = new Job();
    job->moveToThread(&thread);

    connect(this, SIGNAL(startJob(int,
                                  QList<QString>,
                                  QString,
                                  QString)),
            job, SLOT(startJob(int,
                                  QList<QString>,
                                  QString,
                                  QString)));
    connect(job, SIGNAL(jobUpdated(int, int)), this, SLOT(onJobUpdated(int, int)));
    connect(job, SIGNAL(jobFinished(QString)), this, SLOT(onJobFinished(QString)));

    thread.start();
}

JobItem::~JobItem()
{
    delete ui;
    thread.quit();
    thread.wait();
}

void JobItem::start(int type,
                    QList<QString> packPaths,
                    QString path,
                    QString destPath)
{
    qDebug("controller start");
    targetPath = destPath;
    setToolTip(QString("点击查看 %1").arg(targetPath));
    emit startJob(type, packPaths, path, destPath);
}

void JobItem::onJobUpdated(int progress, int total)
{
    job_status->setText(QString("%1 of %2").arg(progress).arg(total));
    job_progress->setValue(int(progress * 100 / total));
}

void JobItem::onJobFinished(QString result)
{
    job_status->setText(result);
    job_progress->setValue(100);
}

void JobItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (targetPath.isEmpty()) return;
    QDesktopServices::openUrl(QUrl(targetPath));
}
