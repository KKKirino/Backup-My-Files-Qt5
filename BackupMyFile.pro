QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileCompressHuffman.cpp \
    disk_file.cpp \
    fake_data.cpp \
    file_list_item.cpp \
    job.cpp \
    job_item.cpp \
    job_page.cpp \
    main.cpp \
    mainwindow.cpp \
    rsa.cpp \
    setting_page.cpp

HEADERS += \
    FileCompressHuffman.h \
    disk_file.h \
    fake_data.h \
    file_list_item.h \
    huffman.h \
    huffman.h \
    huffman2.hpp \
    job.h \
    job_item.h \
    job_page.h \
    mainwindow.h \
    rsa.h \
    setting_page.h

FORMS += \
    file_list_item.ui \
    job_item.ui \
    job_page.ui \
    mainwindow.ui \
    setting_page.ui

TRANSLATIONS += \
    BackupMyFile_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
