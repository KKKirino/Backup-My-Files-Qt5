#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>

#include "settingpage.h"
#include "jobpage.h"
#include "fakedata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;

    // 子页面
    JobPage *jobPage;
    SettingPage *settingPage;

    // 页面展示区域
    QStackedWidget *pageContent;

    // 文件树
    FakeFileTree* root;
    QVector<FakeFileTree*> path;

    void buildHomePageList();
    void pushPath(int i);
    void popPath(int d = 1);

private slots:
    void on_toolButton_2_clicked();
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void on_back_button_triggered(QAction *arg1);
    void on_back_button_clicked();
};

#endif // MAINWINDOW_H
