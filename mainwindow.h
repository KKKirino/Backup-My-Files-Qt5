#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedWidget>
#include <QSet>
#include <QFileDialog>
#include <QInputDialog>

#include "setting_page.h"
#include "job_page.h"
#include "fake_data.h"
#include "disk_file.h"

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
    QFileInfo path;
    QSet<int> selectedPath;
    void togglePath(int index);
    void selectPath(int index);
    void unselectPath(int index);
    void selectAll();
    void clearSelected();
    void updateSelectStatus();

    void buildHomePageList();
    void pushPath(int i);
    void popPath(int d = 1);
    QFileInfoList getChildren();
    QList<QString> getSelectedPath();
    QString getJobName(QList<QString> filePaths);
    void gotoJobPage();

private slots:
    void on_toolButton_2_clicked();
    void on_toolButton_clicked();
    void on_pushButton_clicked();
    void on_back_button_triggered(QAction *arg1);
    void on_back_button_clicked();
    void on_toolButton_3_clicked();
    void on_path_textChanged(const QString &arg1);
    void on_path_returnPressed();
    void on_select_all_btn_clicked();
    void on_unselect_all_btn_clicked();
    void on_pack_btn_clicked();
    void on_backup_btn_clicked();
    void on_unzip_btn_clicked();
    void on_unpack_btn_clicked();
    void on_encrypt_btn_clicked();
    void on_decrypt_btn_clicked();
    void on_restore_btn_clicked();
    void on_zip_btn_clicked();
    void on_unzip_btn_clicked(bool checked);
};

#endif // MAINWINDOW_H
