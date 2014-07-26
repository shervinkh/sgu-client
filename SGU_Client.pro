#-------------------------------------------------
#
# Project created by QtCreator 2012-11-16T11:43:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SGU_Client
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x


SOURCES += \
    filedownloader.cpp \
    main.cpp \
    accountprocessor.cpp \
    statusprocessor.cpp \
    statusview.cpp \
    accountmanager.cpp \
    sguclient.cpp \
    friendactivity.cpp \
    onlinestatus.cpp \
    competitionview.cpp \
    competition.cpp \
    profileinfo.cpp \
    competitiondetail.cpp \
    profilestatus.cpp \
    suggestion.cpp \
    suggestionview.cpp

HEADERS  += \
    filedownloader.h \
    accountprocessor.h \
    statusprocessor.h \
    statusview.h \
    accountmanager.h \
    sguclient.h \
    friendactivity.h \
    onlinestatus.h \
    competitionview.h \
    competition.h \
    profileinfo.h \
    competitiondetail.h \
    profilestatus.h \
    suggestion.h \
    suggestionview.h
