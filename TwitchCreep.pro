#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T19:54:41
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwitchCreep
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           json.cpp \
    networkoperations.cpp \
    database.cpp \
    requesthandler.cpp

HEADERS  += mainwindow.h \
            json.h \
    networkoperations.h \
    database.h \
    requesthandler.h

FORMS    += mainwindow.ui
