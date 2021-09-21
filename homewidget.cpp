#include "homewidget.h"
#include "ui_home.h"

HomeWidget::HomeWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

HomeWidget::~HomeWidget()
{
    delete ui;
}
