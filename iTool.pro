#-------------------------------------------------
#
# Project created by QtCreator 2014-03-14T22:10:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    packetprocessor.cpp \
    handshake.cpp \
    networkthread.cpp \
    packet.cpp

HEADERS  += mainwindow.h \
    packetprocessor.h \
    handshake.h \
    networkthread.h \
    packet.h

FORMS    += mainwindow.ui
