#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "homewidget.h"

#include <QCheckBox>

class FileListItem : public QWidget {
public:
    int index;

    FileListItem (int i) {
        index = i;

        QHBoxLayout* hbox = new QHBoxLayout();
        hbox->setMargin(0);

        QLabel* icon = new QLabel();
        icon->setPixmap(QPixmap(i % 2 ? ":/assets/icons/file.svg" : ":/assets/icons/folder-close.svg").scaled(20, 20));
        icon->setMaximumSize(40, 20);
        icon->setObjectName("file_item_icon");
        icon->setAlignment(Qt::AlignCenter);
        hbox->addWidget(icon);

        QLabel* text = new QLabel();
        text->setText(QString("文件 %1").arg(i));
        text->setObjectName("file_name");
        hbox->addWidget(text);

        QCheckBox* cbox = new QCheckBox(this);
        cbox->setMaximumWidth(30);
        hbox->addWidget(cbox);

        setLayout(hbox);
        setMinimumHeight(40);
        setObjectName("file_list_item");
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , page(new QLabel)
    , home_page(new HomeWidget)
{
    ui->setupUi(this);

    buildHomePageList<FileListItem>(100);
}

template<class T>
void MainWindow::buildHomePageList(int n) {
    auto file_list_content = this->findChild<QWidget *>("file_list_content");

    QGridLayout *pLayout = new QGridLayout();
    pLayout->setMargin(0);
    for(int i = 0; i < n; i++)
        {
            T* child = new T(i);
            pLayout->addWidget(child);
        }
    file_list_content->setLayout(pLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

