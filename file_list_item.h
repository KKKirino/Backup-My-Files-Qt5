#ifndef FILELISTITEM_H
#define FILELISTITEM_H

#include <QWidget>

#include "mainwindow.h"
#include "fake_data.h"
#include "disk_file.h"

namespace Ui {
class FileListItem;
}

class FileListItem : public QWidget
{
    Q_OBJECT

public:
    explicit FileListItem(int i, QFileInfo item, bool selected = false, MainWindow *parent = nullptr);
    ~FileListItem();
    MainWindow* parent;
    int index;
    QFileInfo item;

private:
    Ui::FileListItem *ui;

protected slots:
    void selectedChanged(int state);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // FILELISTITEM_H
