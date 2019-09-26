#ifndef STRATEGYTIEKUANGSHISIMPLEEX2_H
#define STRATEGYTIEKUANGSHISIMPLEEX2_H

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"
#include "backtesting/backtestingconfig.h"
#include "WindowedMaxMin.h"

class StrategyTieKuangShiSimpleEx2 : public Strategy
{
public:
    StrategyTieKuangShiSimpleEx2(TradeGatewayPtr pTradeGatewayforSimple);
    virtual ~StrategyTieKuangShiSimpleEx2() {}

    virtual void init(TradeGatewayPtr pTradeGateway) override;
    virtual void onTick(Tick &tick) override;
    virtual void onBar(KLineDataType &bar) override;
    virtual void onTrade(Trade &trade) override;
    virtual void onOrder(OrderRsp &order) override;
    virtual std::string name() override;
    virtual std::string instrumentType() override;

private:
    int getCloseMinute(KLineDataType &bar);
    void closePosition(int volume);
    void closeAllPosition();
    void openPosition(int volume);
    bool crossDK();
    bool crossKD();

    void BPK(int volume);
    void SPK(int volume);
    void SP(int volume);
    void BP(int volume);
    void SK(int volume);
    void BK(int volume);

    int min(int a, int b);

private:
    TradeGatewayPtr tradeGatewayPtr_;

    VecPtr closeVecPtr_;

    double currentDiff_, lastDiff_;

    MA maClose5Tracker;
    MA maClose6Tracker;
    MA maClose20Tracker;
    MA maClose250Tracker;

    VecPtr captialVecPtr_;
    MA maCapitalTracker_;

    int baselVolume_ = 1;
    int adjVolume_ = 0;

    double maxPosCross_ = 0.0;
    double minNegCross_ = 0.0;

    BacktestingConfig* backtestingConfig;

    double lastK_;
    double currentK_;
    double lastD_;
    double currentD_;
    double lastRsv_;
    WindowedMaxMin windowedMaxMin_;

    TradeGatewayPtr tradeGatewayForSimplePtr_;
    VecPtr simpleStrategyCaptialVecPtr_;
    MA maSimpleStrategyCapital250Tracker_;
    double maxSimpleStrategyCapital_;
    double minBacktrackCross_ = 0.0;
    bool addLot2Trigerred = false;
};

#endif // STRATEGYTIEKUANGSHISIMPLEEX_H
