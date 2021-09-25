#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QTime>

class FileListItem : public QWidget {
public:
    MainWindow* parent;
    int index;
    FakeFileTree* item;

    explicit FileListItem (int i, FakeFileTree* item, MainWindow* parent = nullptr) : parent(parent), index(i), item(item) {
        QHBoxLayout* hbox = new QHBoxLayout();
        hbox->setMargin(0);

        QLabel* icon = new QLabel();
        icon->setPixmap(QPixmap(!item->isFolder ? ":/assets/icons/file.svg" : ":/assets/icons/folder-close.svg").scaled(20, 20));
        icon->setMaximumSize(40, 20);
        icon->setObjectName("file_item_icon");
        icon->setAlignment(Qt::AlignCenter);
        hbox->addWidget(icon);

        QLabel* text = new QLabel();
        text->setText(item->name + (item->isFolder ? "/" : ""));
        text->setObjectName("file_name");
        hbox->addWidget(text);

        QCheckBox* cbox = new QCheckBox(this);
        cbox->setMaximumWidth(30);
        hbox->addWidget(cbox);

        setLayout(hbox);
        setMinimumHeight(40);
        setObjectName("file_list_item");
    }

protected:
    void mouseReleaseEvent(QMouseEvent *event) {
        qDebug("hello");
        if (parent != nullptr && item->isFolder) {
            parent->pushPath(index);
        }
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , jobPage(new JobPage)
    , settingPage(new SettingPage)
    , root(new FakeFileTree)
{
    ui->setupUi(this);

    pageContent = this->findChild<QStackedWidget *>("page_content");
    pageContent->addWidget(jobPage);
    pageContent->addWidget(settingPage);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    path.append(root);
    buildHomePageList();
}

void MainWindow::buildHomePageList() {
    auto file_list_content = this->findChild<QWidget *>("file_list_content");

    QLayout *pLayout = file_list_content->layout() ? file_list_content->layout() : new QVBoxLayout();

    pLayout->setAlignment(Qt::AlignTop);

    int oldChildCount = pLayout->count();
    qDebug("removing children %d", oldChildCount);
    for (int i = oldChildCount - 1; i >= 0; i -= 1) {
        pLayout->itemAt(i)->widget()->setVisible(false);
        pLayout->removeItem(pLayout->itemAt(i));
    }

    oldChildCount = pLayout->count();

    pLayout->setMargin(0);
    for(int i = 0; i < path.last()->children.length(); i++)
    {
        FileListItem* child = new FileListItem(i, path.last()->children[i], this);
        if (i < oldChildCount) {
            auto oldWidget = pLayout->itemAt(i)->widget();
            pLayout->replaceWidget(oldWidget, child);
        }
        pLayout->addWidget(child);
    }
    qDebug("pLayout %d", pLayout->count());
    file_list_content->setLayout(pLayout);

    // update text
    QString pathStr;
    for (auto p : path) {
        pathStr += "/" + p->name;
    }
    this->findChild<QLineEdit*>("path")->setText(pathStr);
}

void MainWindow::pushPath(int i) {
    if (path.length() < 1 || i >= path.last()->children.length()) return;
    path.push_back(path.last()->children[i]);
    buildHomePageList();
}

void MainWindow::popPath(int d) {
    while (path.length() > 1 && d > 0) {
        path.pop_back();
        d -= 1;
    }
    buildHomePageList();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_toolButton_2_clicked()
{
    pageContent->setCurrentIndex(1);
}

void MainWindow::on_toolButton_clicked()
{
    pageContent->setCurrentIndex(0);
}

void MainWindow::on_pushButton_clicked()
{
}

void MainWindow::on_back_button_triggered(QAction *arg1)
{
    popPath();
}

void MainWindow::on_back_button_clicked()
{
    popPath();
}
