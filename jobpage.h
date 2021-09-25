#ifndef JOBPAGE_H
#define JOBPAGE_H

#include <QWidget>

namespace Ui {
class JobPage;
}

class JobPage : public QWidget
{
    Q_OBJECT

public:
    explicit JobPage(QWidget *parent = nullptr);
    ~JobPage();

private:
    Ui::JobPage *ui;
};

#endif // JOBPAGE_H
