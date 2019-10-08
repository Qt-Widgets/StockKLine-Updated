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
    backtesting/BacktestingTradeGateway.cpp \
    backtesting/EventEngine.cpp \
    backtesting/MA.cpp \
    backtesting/StrategyTieKuangShiEx.cpp \
    backtesting/StrategyTieKuangShiSimple.cpp \
    backtesting/StrategyTieKuangShiSimpleEx.cpp \
    backtesting/Utility.cpp \
    backtesting/WindowedMaxMin.cpp \
    backtesting/backtestingconfig.cpp \
    backtesting/backtestingdriver.cpp \
    backtesting/backtestingdriverforjiaotansimpleex.cpp \
    backtesting/backtestingdriverforsimpleex.cpp \
    backtesting/backtestingdriverforsimpleex2.cpp \
    backtesting/strategyjiaotansimple.cpp \
    backtesting/strategyjiaotansimpleex.cpp \
    backtesting/strategytiekuangshisimple2.cpp \
    backtesting/strategytiekuangshisimpleex2.cpp \
    backtestingtab.cpp \
    bottomtimegrid.cpp \
    capitalaveragediffgrid.cpp \
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
    topbacktestingmenu.cpp \
    topbacktestingsimpleexmenu.cpp \
    topinstrumentsummary.cpp

HEADERS  += mainwindow.h \
    autogrid.h \
    backtesting/BacktestingEngine.h \
    backtesting/BacktestingTradeGateway.h \
    backtesting/BlockingQueue.h \
    backtesting/Event.h \
    backtesting/EventEngine.h \
    backtesting/KlineData.h \
    backtesting/MA.h \
    backtesting/Strategy.h \
    backtesting/StrategyTieKuangShiEx.h \
    backtesting/StrategyTieKuangShiSimple.h \
    backtesting/StrategyTieKuangShiSimpleEx.h \
    backtesting/ThostFtdcMdApi.h \
    backtesting/ThostFtdcTraderApi.h \
    backtesting/ThostFtdcUserApiDataType.h \
    backtesting/ThostFtdcUserApiStruct.h \
    backtesting/TradeGateway.h \
    backtesting/Utility.h \
    backtesting/WindowedMaxMin.h \
    backtesting/backtestingconfig.h \
    backtesting/backtestingdriver.h \
    backtesting/backtestingdriverforjiaotansimpleex.h \
    backtesting/backtestingdriverforsimpleex.h \
    backtesting/backtestingdriverforsimpleex2.h \
    backtesting/date.h \
    backtesting/strategyjiaotansimple.h \
    backtesting/strategyjiaotansimpleex.h \
    backtesting/strategytiekuangshisimple2.h \
    backtesting/strategytiekuangshisimpleex2.h \
    backtestingtab.h \
    bottomtimeGrid.h \
    bottomtimegrid.h \
    capitalaveragediffgrid.h \
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
    topbacktestingmenu.h \
    topbacktestingsimpleexmenu.h \
    topinstrumentsummary.h

FORMS    += mainwindow.ui



TRANSLATIONS = myI18N_zh_CN.ts

DISTFILES += \
    dataKLine.txt \
    scripts/Kline_Generator.py \
    scripts/Kline_Generator2.py
