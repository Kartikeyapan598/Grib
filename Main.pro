QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/DownloadManager.cpp \
    src/csvloader.cpp \
    src/tableview.cpp \
    src/textprogressbar.cpp \
    src\Square.cpp \
    src\form.cpp \
    src\main.cpp \
    src\mainwindow.cpp \
    src\points.cpp \
    src\rendertypes.cpp

HEADERS += \
    src/DownloadManager.h \
    src/csvloader.h \
    src/tableview.h \
    src/textprogressbar.h \
    src\Square.h \
    src\Utils.h \
    src\form.h \
    src\mainwindow.h \
    src\points.h \
    src\rendertypes.h

FORMS += \
    src\form.ui \
    src\mainwindow.ui \
    src\rendertypes.ui


