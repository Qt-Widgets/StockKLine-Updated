#ifndef BACKTESTINGCONFIG_H
#define BACKTESTINGCONFIG_H

#include <QString>
#include <vector>

#include "backtestingdriverforjiaotansimpleex2.h"
#include "backtestingdriverforluowengangsimpleex.h"
#include "backtestingdriverforsimpleex2.h"
#include "backtestingdriverinterface.h"
#include "backtestingdriverforjiaotansimpleex2.h"
#include "backtestingdriverforluowengangsimpleex.h"
#include "backtestingdriverforsimpleex2.h"

class BacktestingConfig
{
public:
    static BacktestingConfig* instance() {
        if (!config) {
            config = new BacktestingConfig();
            (config->backtestingDrivers).push_back(std::make_shared<BacktestingDriverForSimpleEx2>());
            (config->backtestingDrivers).push_back(std::make_shared<BacktestingDriverForJiaoTanSimpleEx2>());
            (config->backtestingDrivers).push_back(std::make_shared<BacktestingDriverForLuoWenGangSimpleEx>());
        }
        return config;
    }

    ~BacktestingConfig() {}

    int capitalPeriod;
    double posThreshold1;
    double posThreshold2;
    double posThreshold3;
    double posThreshold4;
    double posThreshold5;
    double negThreshold1;
    double negThreshold2;
    double negThreshold3;

    int posLotThreshold1;
    int posLotThreshold2;
    int posLotThreshold3;
    int posLotThreshold4;
    int posLotThreshold5;
    int negLotThreshold1;
    int negLotThreshold2;
    int negLotThreshold3;

    int baseLot;
    int totalAdjLot;

    bool enableCapitalAjdustment;

    // for SimpleEx
    double addLotDiffThreshold1;
    double addLotBacktrackThreshold1;
    double addLotDiffThreshold2;
    double addLotBacktrackThreshold2;
    double decLotDiffThreshold1;
    double decLotDiffThreshold2;

    double tieKuangShiN = 40.0;

    /////////////////////////////////////////////////////////////////////////////////
    int testEngineIndex = 1;

    std::vector<QString> marketDataFiles = {
        QStringLiteral("TieKuangShi_15min_I.csv"),
        QStringLiteral("JiaoTan_15min_JL9.csv"),
        QStringLiteral("LuoWenGang_15min_RB.csv")
    };

    std::vector<QString> backtestingSimpleFiles = {
        QStringLiteral("TieKuangshiEx_15min_Backtesting_Stats_Simple.csv"),
        QStringLiteral("JiaoTan_15min_Backtesting_Stats_Simple.csv"),
        QStringLiteral("LuoWenGang_15min_Backtesting_Stats_Simple.csv")
    };

    std::vector<QString> backtestingSimpleExFiles = {
        QStringLiteral("TieKuangshiEx_15min_Backtesting_Stats.csv"),
        QStringLiteral("JiaoTan_15min_Backtesting_Stats.csv"),
        QStringLiteral("LuoWenGang_15min_Backtesting_Stats.csv")
    };

    std::vector<std::shared_ptr<BacktestingDriverInterface>> backtestingDrivers;

private:
    BacktestingConfig() {
        capitalPeriod = 250;
        posThreshold1 = 31.0;
        posThreshold2 = 42.0;
        posThreshold3 = 52.0;
        posThreshold4 = 65.0;
        posThreshold5 = 92.0;
        negThreshold1 = -40.0;
        negThreshold2 = -54.0;
        negThreshold3 = -84.0;

        posLotThreshold1 = 1;
        posLotThreshold2 = 1;
        posLotThreshold3 = 1;
        posLotThreshold4 = 1;
        posLotThreshold5 = 1;
        negLotThreshold1 = 2;
        negLotThreshold2 = 1;
        negLotThreshold3 = 1;

        baseLot = 1;

        enableCapitalAjdustment = true;

        addLotDiffThreshold1 = -3000.0;
        addLotBacktrackThreshold1 = -7000.0;
        addLotDiffThreshold2 = -50.0;
        addLotBacktrackThreshold2 = -10000.0;
        decLotDiffThreshold1 = 3900.0;
        decLotDiffThreshold2 = 9200.0;
    }
    BacktestingConfig(BacktestingConfig& ) = delete;
    BacktestingConfig& operator=(const BacktestingConfig& ) = delete;

    static BacktestingConfig* config;
};

#endif // BACKTESTINGCONFIG_H
