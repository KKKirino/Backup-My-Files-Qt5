#include "jobpage.h"
#include "ui_jobpage.h"

JobPage::JobPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JobPage)
{
    ui->setupUi(this);
}

JobPage::~JobPage()
{
    delete ui;
}
