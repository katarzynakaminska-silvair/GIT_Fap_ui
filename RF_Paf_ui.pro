#-------------------------------------------------
#
# Project created by QtCreator 2015-08-03T13:47:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = RF_Paf_ui
TEMPLATE = app


SOURCES += main.cpp\
        rf_paf.cpp \
    protocol.cpp \
    command.cpp

HEADERS  += rf_paf.h \
    protocol.h \
    command.h

FORMS    += rf_paf.ui
