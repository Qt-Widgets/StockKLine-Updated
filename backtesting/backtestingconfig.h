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

private:
    BacktestingConfig() {
        capitalPeriod = 250;
        posThreshold1 = 3100.0;
        posThreshold2 = 4200.0;
        posThreshold3 = 5200.0;
        posThreshold4 = 6500.0;
        posThreshold5 = 9200.0;
        negThreshold1 = -2000.0;
        negThreshold2 = -2700.0;
        negThreshold3 = -3700.0;
    }
    BacktestingConfig(BacktestingConfig& ) = delete;
    BacktestingConfig& operator=(const BacktestingConfig& ) = delete;

    static BacktestingConfig* config;
};

#endif // BACKTESTINGCONFIG_H
