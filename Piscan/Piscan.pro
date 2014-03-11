#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T16:47:49
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Piscan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glscanwidget.cpp \
    scandraid.cpp \
    exportfile.cpp \
    pointcloud.cpp

HEADERS  += mainwindow.h \
    glscanwidget.h \
    scandraid.h \
    exportfile.h \
    pointcloud.h

#LIBS += -L/usr/lib -ljpeg

FORMS    += mainwindow.ui
