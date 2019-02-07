#-------------------------------------------------
#
# Project created by QtCreator 2019-02-01T14:34:49
#
#-------------------------------------------------

QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DESTDIR = ../build_Pro
TARGET = Project_Test
CONFIG += c++11

TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwgt.cpp \
    network/tcpclient.cpp

HEADERS += \
        mainwgt.h \
    network/tcpclient.h

FORMS += \
        mainwgt.ui

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../headers
