#ifndef JOBPAGE_H
#define JOBPAGE_H

#include <QWidget>
#include <QVBoxLayout>

#include "job_item.h"

namespace Ui {
class JobPage;
}

class JobPage : public QWidget
{
    Q_OBJECT

public:
    explicit JobPage(QWidget *parent = nullptr);
    ~JobPage();

    Ui::JobPage *ui;

    QWidget* job_list;
    QVBoxLayout* job_list_content;
    QList<JobItem*> jobs;

    void buildJobList();
    JobItem* createJob(QString name, QString status, int progress);
};

#endif // JOBPAGE_H
