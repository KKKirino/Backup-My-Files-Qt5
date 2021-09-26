#include "file_list_item.h"
#include "ui_file_list_item.h"

FileListItem::FileListItem(int i, QFileInfo item, bool selected, MainWindow *parent) :
    parent(parent),
    index(i),
    item(item),
    ui(new Ui::FileListItem)
{
    ui->setupUi(this);

    QLabel* icon = findChild<QLabel*>("file_item_icon");
    icon->setPixmap(QPixmap(item.isFile() ? ":/assets/icons/file.svg" : ":/assets/icons/folder-close.svg").scaled(20, 20));
    icon->setMaximumSize(40, 20);
    icon->setAlignment(Qt::AlignCenter);

    QLabel* text = findChild<QLabel*>("file_name");
    text->setText(item.fileName() + (item.isDir() ? "/" : ""));

    QCheckBox* cbox = findChild<QCheckBox*>("file_checker");
    cbox->setMaximumWidth(30);
    cbox->setCheckState(selected ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    connect(cbox, SIGNAL(stateChanged(int)), this, SLOT(selectedChanged(int)));

    // 目录 .. 不能选中
    cbox->setVisible(!item.fileName().startsWith(".."));

    setMinimumHeight(40);
    setStyleSheet(selected ? ".QWidget {background: rgba(0, 0, 0, 0.05);}" : "");
}

void FileListItem::mouseReleaseEvent(QMouseEvent *event) {
    if (parent == nullptr) return;
    if ( item.isDir()) {
        parent->pushPath(index);
    }
    if (item.isFile()) {
        parent->togglePath(index);
        parent->buildHomePageList();
    }
}

void FileListItem::selectedChanged(int state)
{
    if (state == Qt::CheckState::Checked) {
        this->parent->selectPath(index);
        setStyleSheet(".QWidget {background: rgba(0, 0, 0, 0.05);}");
    } else {
        this->parent->unselectPath(index);
        setStyleSheet("");
    }
}

FileListItem::~FileListItem()
{
    delete ui;
}
