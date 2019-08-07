#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T19:30:58
#
#-------------------------------------------------

QT       += core gui




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StockKLine
TEMPLATE = app




# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    backtestingtab.cpp \
    bottomtimegrid.cpp \
    capitallinegrid.cpp \
    datadetailbox.cpp \
    datawidget.cpp \
        mainwindow.cpp \
    autogrid.cpp \
    klinegrid.cpp \
    datafile.cpp \
    marketdataadmin.cpp \
    marketdatasplitter.cpp \
    marketdatatab.cpp \
    showdetail.cpp \
    kvolumegrid.cpp \
    topinstrumentsummary.cpp

HEADERS  += mainwindow.h \
    autogrid.h \
    backtestingtab.h \
    bottomtimegrid.h \
    capitallinegrid.h \
    datadetailbox.h \
    datawidget.h \
    klinegrid.h \
    datafile.h \
    marketdataadmin.h \
    marketdatasplitter.h \
    marketdatatab.h \
    showdetail.h \
    kvolumegrid.h \
    topinstrumentsummary.h

FORMS    += mainwindow.ui



TRANSLATIONS = myI18N_zh_CN.ts

DISTFILES += \
    dataKLine.txt
