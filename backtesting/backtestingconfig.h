#ifndef BACKTESTINGCONFIG_H
#define BACKTESTINGCONFIG_H

class BacktestingConfig
{
public:
    static BacktestingConfig* instance() {
        if (!config) {
            config = new BacktestingConfig();
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
