#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file_list_item.h"

#include <QCheckBox>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , jobPage(new JobPage)
    , settingPage(new SettingPage)
{
    ui->setupUi(this);

    auto cs = QDir::currentPath();
    path = QFileInfo(cs);

    pageContent = this->findChild<QStackedWidget *>("page_content");
    pageContent->addWidget(jobPage);
    pageContent->addWidget(settingPage);

    pageContent->setCurrentIndex(0);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    buildHomePageList();
}

void MainWindow::buildHomePageList() {
    updateSelectStatus(); // 更新文件选中状态
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

    auto fileList = getChildren();

    // 下标从 1 开始，过滤掉 . 目录
    for(int i = 1; i < fileList.count(); i++)
    {
        FileListItem* child = new FileListItem(i, fileList[i], selectedPath.contains(i), this);
        if (i < oldChildCount) {
            auto oldWidget = pLayout->itemAt(i)->widget();
            pLayout->replaceWidget(oldWidget, child);
        }
        pLayout->addWidget(child);
    }
    qDebug("pLayout %d", pLayout->count());
    file_list_content->setLayout(pLayout);

    // update text
    QString pathStr = path.absoluteFilePath();
    this->findChild<QLineEdit*>("path")->setText(pathStr);
}

void MainWindow::pushPath(int i) {
    auto fileList = getChildren();
    if (fileList.length() < 1 || i >= fileList.count()) return;
    path = QFileInfo(fileList[i]);
    clearSelected();
    buildHomePageList();
}

void MainWindow::popPath(int d) {
    while (!path.isRoot() && d > 0 && path.dir().exists()) {
        path = QFileInfo(path.dir().path());
        d -= 1;
    }
    clearSelected();
    buildHomePageList();
}

QFileInfoList MainWindow::getChildren()
{
    auto current = QDir(path.filePath());
    return current.entryInfoList(QDir::NoFilter, QDir::DirsFirst);
}

void MainWindow::selectAll()
{
    for (int i = 2; i < getChildren().count(); i += 1)
    {
        selectedPath.insert(i);
    }
    updateSelectStatus();
}

void MainWindow::togglePath(int index)
{
    if (selectedPath.contains(index)) unselectPath(index);
    else selectPath(index);
}

void MainWindow::selectPath(int index)
{
    selectedPath.insert(index);
    updateSelectStatus();
}

void MainWindow::unselectPath(int index)
{
    selectedPath.remove(index);
    updateSelectStatus();
}

void MainWindow::clearSelected()
{
    selectedPath.clear();
}

void MainWindow::updateSelectStatus()
{
    auto label = findChild<QLabel*>("select_status");
    label->setText(QString("已选中 %1 个项目").arg(selectedPath.count()));
}

QList<QString> MainWindow::getSelectedPath()
{
    QList<QString> filePaths;
    auto fileLists = getChildren();
    for (auto i : selectedPath)
    {
        filePaths.append(fileLists[i].absoluteFilePath());
    }
    return filePaths;
}

QString MainWindow::getJobName(QList<QString> filePaths)
{
    QString shortName = QFileInfo(filePaths[0]).fileName();
    QString name = QString("%1 等 %2 个文件").arg(shortName).arg(filePaths.count());
    return name;
}

void MainWindow::gotoJobPage()
{
    pageContent->setCurrentIndex(1);
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

void MainWindow::on_toolButton_3_clicked()
{
    pageContent->setCurrentIndex(2);
}

// 路径改变时，切换路径
void MainWindow::on_path_textChanged(const QString &inputPath)
{
    auto newPath = QFileInfo(inputPath);
    if (newPath.exists()) {
        path = newPath;
    }
    buildHomePageList();
}

void MainWindow::on_path_returnPressed()
{

}

void MainWindow::on_select_all_btn_clicked()
{
    selectAll();
    buildHomePageList();
}

void MainWindow::on_unselect_all_btn_clicked()
{
    clearSelected();
    buildHomePageList();
}

void MainWindow::on_pack_btn_clicked()
{
    // 打包文件
    if (selectedPath.count() == 0) return;
    auto filePaths = getSelectedPath();
    QString name = getJobName(filePaths);

    auto job = jobPage->createJob(name, "等待开始", 0);

    // 选择打包路径
    QString destPath = QFileDialog::getExistingDirectory(this, tr("请选择打包文件存放位置"));
    job->start(Job::JobType::PACK, filePaths, "", destPath);
    gotoJobPage();
}

void MainWindow::on_backup_btn_clicked()
{
    // 备份文件
    if (selectedPath.count() == 0) return;
    auto filePaths = getSelectedPath();
    QString name = getJobName(filePaths);
    auto job = jobPage->createJob(name, "等待开始", 0);

    // 选择备份路径
    QString destPath = QFileDialog::getExistingDirectory(this, tr("请选择备份位置"));
    job->start(Job::JobType::BACKUP, filePaths, "", destPath);
    gotoJobPage();
}
