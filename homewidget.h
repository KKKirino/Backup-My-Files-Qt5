#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "ui_home.h"

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

private:
    Ui::Form *ui;

signals:

};

#endif // HOMEWIDGET_H
