#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T16:47:49
#
#-------------------------------------------------

QT       += core gui opengl serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Piscan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glscanwidget.cpp \
    exportfile.cpp \
    pointcloud.cpp \
    devicedialog.cpp \
    processscan.cpp \
    filetransfer.cpp

HEADERS  += mainwindow.h \
    glscanwidget.h \
    exportfile.h \
    pointcloud.h \
    devicedialog.h \
    processscan.h \
    filetransfer.h

FORMS    += mainwindow.ui \
    devicedialog.ui
