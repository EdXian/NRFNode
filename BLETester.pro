#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T07:38:29
#
#-------------------------------------------------

QT       += core gui bluetooth
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BLETester
TEMPLATE = app


SOURCES += main.cpp\
    bleservices/config_service.cpp \
    bleservices/environment_service.cpp \
    bleservices/motion_service.cpp \
    bleservices/sound_service.cpp \
    bleservices/thingy_manager.cpp \
    bleservices/ui_service.cpp \
        mainwindow.cpp \
    bleinterface.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    bleinterface.h \
    bleservices/config_service.h \
    bleservices/environment_service.h \
    bleservices/motion_service.h \
    bleservices/sound_service.h \
    bleservices/thingy_manager.h \
    bleservices/thingy_service.h \
    bleservices/ui_service.h \
    lib-qt-qml-tricks/src/qqmlhelpers.h \
    qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
