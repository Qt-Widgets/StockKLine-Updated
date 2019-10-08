﻿#include <iostream>
#include "BacktestingTradeGateway.h"
#include "backtestingdriverforjiaotansimpleex.h"
#include "strategyjiaotansimple.h"
#include "strategyjiaotansimpleex.h"

void BacktestingDriverForJiaoTanSimpleEx::test()
{
    //
    // create the test strategy for simple
    //
    EventEnginePtr eventEngineForSimplePtr = std::make_shared<EventEngine>();

//    BacktestingTradeGatewayPtr 	backtestingTradeGatewayForSimplePtr =
//        std::make_shared<BacktestingTradeGateway>(eventEngineForSimplePtr, "E:\\cbm\\startup\\qihuoshuju_good\\TieKuangShiEx_15min_Backtesting_Stats_Simple.csv");

    BacktestingTradeGatewayPtr 	backtestingTradeGatewayForSimplePtr =
        std::make_shared<BacktestingTradeGateway>(eventEngineForSimplePtr, "JiaoTan_15min_Backtesting_Stats_Simple.csv");

    backtestingTradeGatewayForSimplePtr->init();
    TradeGatewayPtr tradeGatewayForSimplePtr = backtestingTradeGatewayForSimplePtr;

    StrategyPtr strategyForSimplePtr = std::make_shared<StrategyJiaoTanSimple>();
    strategyForSimplePtr->init(tradeGatewayForSimplePtr);


    //
    // create the test strategy for the simple ex
    //

    EventEnginePtr eventEnginePtr = std::make_shared<EventEngine>();

//    BacktestingTradeGatewayPtr 	backtestingTradeGatewayPtr =
//        std::make_shared<BacktestingTradeGateway>(eventEnginePtr, "E:\\cbm\\startup\\qihuoshuju_good\\TieKuangShiEx_15min_Backtesting_Stats.csv");

    BacktestingTradeGatewayPtr 	backtestingTradeGatewayPtr =
        std::make_shared<BacktestingTradeGateway>(eventEnginePtr, "JiaoTan_15min_Backtesting_Stats.csv");

    backtestingTradeGatewayPtr->init();
    TradeGatewayPtr tradeGatewayPtr = backtestingTradeGatewayPtr;

    StrategyPtr strategyPtr = std::make_shared<StrategyJiaoTanSimpleEx>(backtestingTradeGatewayForSimplePtr);
    strategyPtr->init(tradeGatewayPtr);

    std::cout << "loading market data ..." << std::endl;
//    KLineVecPtr klines = loadKline("JI", "E:\\cbm\\startup\\qihuoshuju_good\\TieKuangShi_15min_I.csv");
    KLineVecPtr klines = loadKline("JI", "JiaoTan_15min_JL9.csv");

    Event event;
    for (auto& bar : *klines) {
        event.type = EventType::BarEvent;
        event.data.kline = *bar;

        backtestingTradeGatewayForSimplePtr->newMarketDataEvent(event);
        strategyForSimplePtr->onBar(*bar);
        backtestingTradeGatewayForSimplePtr->recordStatus();

        backtestingTradeGatewayPtr->newMarketDataEvent(event);
        strategyPtr->onBar(*bar);
        backtestingTradeGatewayPtr->recordStatus();
    }
}

std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> BacktestingDriverForJiaoTanSimpleEx::loadKline(const std::string &instrumentID, std::string filePath)
{
    std::ifstream ins = std::ifstream(filePath, std::ios::in);
    if (!ins) {
        std::cout << "Cannot find file: " << filePath << std::endl;
        return nullptr;
    }

    if (!ins.is_open()) {
        std::cout << "Cannot open file: " << filePath << std::endl;
        return nullptr;
    }

    std::vector<std::string> files;
    files.push_back(filePath);

    return std::move(loadKline(files));
}

std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> BacktestingDriverForJiaoTanSimpleEx::loadKline(std::vector<std::string>& files)
{
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> data = std::make_unique<std::vector<std::shared_ptr<KLineDataType>>>();
    for (auto iter = files.rbegin(); iter != files.rend(); ++iter) {
        std::string f = *iter;
        std::cout << "Loading market data from the file: " << f << std::endl;
        std::ifstream ins = std::ifstream(f, std::ios::in);

        std::vector<std::string> columns;
        std::string line;
        while (!ins.eof()) {
            std::getline(ins, line);
            if (line.empty()) {
                continue;
            }
            columns.clear();
            split(line, ',', columns);

            std::shared_ptr<KLineDataType> kline = std::make_shared<KLineDataType>();
            strcpy(kline->update_time, columns[0].c_str());
            kline->open_price = std::stod(columns[1]);
            kline->high_price = std::stod(columns[2]);
            kline->low_price = std::stod(columns[3]);
            kline->close_price = std::stod(columns[4]);
            kline->volume = std::stoi(columns[5]);
            data->push_back(kline);
        }
        ins.close();
        std::cout << "Finished loading market data from the file: " << f << std::endl;
    }
    return std::move(data);
}

void BacktestingDriverForJiaoTanSimpleEx::split(const std::string& s, char c, std::vector<std::string>& v)
{
    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);

    while (j != std::string::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
            v.push_back(s.substr(i, s.length()));
    }
}
