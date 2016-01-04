#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T15:30:35
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Barquette
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    barquette.cpp \
    modbustcp.cpp \
    tapis.cpp \
    partieoperative.cpp \
    capteurs.cpp

HEADERS  += mainwindow.h \
    barquette.h \
    modbustcp.h \
    tapis.h \
    partieoperative.h \
    capteurs.h

FORMS    += mainwindow.ui
