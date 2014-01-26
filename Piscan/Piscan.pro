#-------------------------------------------------
#
# Project created by QtCreator 2014-01-11T16:47:49
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScanDraid
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ScanDraiD.cpp \
    Jpeg.cpp \
    Image.cpp \
    glscanwidget.cpp

HEADERS  += mainwindow.h \
    ScanDraiD.hpp \
    Jpeg.hpp \
    Image.hpp \
    glscanwidget.h

LIBS += -L/usr/lib -ljpeg

FORMS    += mainwindow.ui
