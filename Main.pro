QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svgwidgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/data.cpp \
    src/form.cpp \
    src/griddata.cpp \
    src/gridder.cpp \
    src/infopanel.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/plotter.cpp \
    src/rendertypes.cpp \
    src/customview.cpp \
    src/customscene.cpp \
    src/DownloadManager.cpp \
    src/textprogressbar.cpp \
    src/csvroutes.cpp \
#    src/uploadmanager.cpp \
#    src/auth.cpp

HEADERS += \
    src/Utils.h \
    src/data.h \
    src/form.h \
    src/griddata.h \
    src/gridder.h \
    src/infopanel.h \
    src/mainwindow.h \
    src/plotter.h \
    src/rendertypes.h \
    src/customview.h \
    src/customscene.h \
    src/DownloadManager.h \
    src/textprogressbar.h \
    src/csvroutes.h \
#    src/uploadmanager.h \
#    src/auth.h


FORMS += \
    src/form.ui \
    src/infopanel.ui \
    src/mainwindow.ui \
    src/rendertypes.ui
#    src/auth.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
