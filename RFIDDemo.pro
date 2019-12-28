#-------------------------------------------------
#
# Project created by QtCreator 2019-10-29T02:12:07
#
#-------------------------------------------------
QT       += core gui network sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFIDDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    rfid.c \
    mysqlmodel.cpp \
    worker.cpp \
    videostream_yuv.c \
    caminfo.c \
    recocard.cpp \
    texttosour.cpp

HEADERS += \
        widget.h \
    iso14443a.h \
    rfid.h \
    mysqlmodel.h \
    worker.h \
    common.h \
    caminfo.h \
    video_yuv.h \
    jpeglib.h \
    jconfig.h \
    jerror.h \
    jmorecfg.h \
    recocard.h \
    texttosour.h

FORMS += \
        widget.ui

LIBS += -L/home/gec/tools/jpglib/lib -ljpeg


