QT += core
QT -= gui

TARGET = Lab1

CONFIG += console
CONFIG -= app_bundle

DEFINES += _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    cop.h \
    tstack.h \
    str_switch.h

DISTFILES +=

deployment.path = $$OUT_PWD/
deployment.files += input.txt \


