QT += core
QT -= gui


CONFIG -= app_bundle

CONFIG += c++11

TARGET = console_test
CONFIG += console


TEMPLATE = app

SOURCES += main.cpp

include(../ussss/qtsingleapplication/qtsinglecoreapplication.pri)
