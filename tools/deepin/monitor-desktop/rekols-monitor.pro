#-------------------------------------------------
#
# Project created by QtCreator 2017-07-31T12:48:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rekols-monitor
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        main_window.cpp \
    utils.cpp \
    memory_monitor.cpp \
    thread.cpp

HEADERS  += main_window.h \
    utils.h \
    memory_monitor.h \
    thread.h

isEmpty(BINDIR):BINDIR=/usr/bin
isEmpty(APPDIR):APPDIR=/usr/share/applications

target.path = $$INSTROOT$$BINDIR
desktop.path = $$INSTROOT$$APPDIR
desktop.files = rekols-monitor.desktop

INSTALLS += target desktop
