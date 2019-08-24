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
        posThreshold1 = 31.0;
        posThreshold2 = 42.0;
        posThreshold3 = 52.0;
        posThreshold4 = 65.0;
        posThreshold5 = 92.0;
        negThreshold1 = -40.0;
        negThreshold2 = -54.0;
        negThreshold3 = -84.0;
    }
    BacktestingConfig(BacktestingConfig& ) = delete;
    BacktestingConfig& operator=(const BacktestingConfig& ) = delete;

    static BacktestingConfig* config;
};

#endif // BACKTESTINGCONFIG_H
