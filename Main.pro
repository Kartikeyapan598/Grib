QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svgwidgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Square.cpp \
    src/data.cpp \
    src/form.cpp \
    src/griddata.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/plotter.cpp \
    src/points.cpp \
    src/rendertypes.cpp \
    src/customview.cpp \
    src/customscene.cpp \
    src/DownloadManager.cpp \
    src/textprogressbar.cpp

HEADERS += \
    src/Square.h \
    src/Utils.h \
    src/data.h \
    src/form.h \
    src/griddata.h \
    src/mainwindow.h \
    src/plotter.h \
    src/points.h \
    src/rendertypes.h \
    src/customview.h \
    src/customscene.h \
    src/DownloadManager.h \
    src/textprogressbar.h


FORMS += \
    src/form.ui \
    src/mainwindow.ui \
    src/rendertypes.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
