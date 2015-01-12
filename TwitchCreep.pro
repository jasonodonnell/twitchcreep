#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T19:54:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwitchCreep
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           json.cpp \
           fileoperations.cpp \
    networkoperations.cpp

HEADERS  += mainwindow.h \
            json.h \
            fileoperations.h \
    networkoperations.h

FORMS    += mainwindow.ui
