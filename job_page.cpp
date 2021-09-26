#include "job_page.h"
#include "ui_job_page.h"
#include "job_item.h"

JobPage::JobPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JobPage),
    job_list(nullptr),
    job_list_content(new QVBoxLayout)
{
    ui->setupUi(this);

    job_list = this->findChild<QWidget*>("job_list");
    job_list->setLayout(job_list_content);
    job_list_content->setAlignment(Qt::AlignTop);
    job_list_content->setDirection(QVBoxLayout::BottomToTop);
    buildJobList();
}

void JobPage::buildJobList()
{
    for (int i = 0; i < 1; i += 1) {
//        auto job = new JobItem(QString("任务%1").arg(i), QString("100 of 200"), qrand() % 100, this);
//        jobs.append(job);
//        job_list_content->addWidget(job);
    }
}

JobItem* JobPage::createJob(QString name, QString status, int progress)
{
    JobItem* job = new JobItem(name, status, progress);
    jobs.append(job);
    job_list_content->addWidget(job);

    return job;
}

JobPage::~JobPage()
{
    delete ui;
}
