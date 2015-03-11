#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T19:54:41
#
#-------------------------------------------------

QT       += core gui network sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwitchCreep
TEMPLATE = app


SOURCES += Sources/database.cpp \
    Sources/json.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/networkoperations.cpp \
    Sources/requesthandler.cpp \
    Sources/timers.cpp

HEADERS  += Headers/database.h \
    Headers/json.h \
    Headers/mainwindow.h \
    Headers/networkoperations.h \
    Headers/requesthandler.h \
    Headers/timers.h \

FORMS    += Forms/mainwindow.ui
