#ifndef STRATEGYTIEKUANGSHISIMPLEEX_H
#define STRATEGYTIEKUANGSHISIMPLEEX_H

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"
#include "backtesting/backtestingconfig.h"

class StrategyTieKuangShiSimpleEx : public Strategy
{
public:
    StrategyTieKuangShiSimpleEx(TradeGatewayPtr pTradeGatewayforSimple);
    virtual ~StrategyTieKuangShiSimpleEx() {}

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

    MA maClose26Tracker;
    MA maClose10Tracker;
    MA maClose250Tracker;

    VecPtr captialVecPtr_;
    MA maCapitalTracker_;

    int baselVolume_ = 1;
    int adjVolume_ = 0;

    double maxPosCross_ = 0.0;
    double minNegCross_ = 0.0;

    BacktestingConfig* backtestingConfig;

    TradeGatewayPtr tradeGatewayForSimplePtr_;
    VecPtr simpleStrategyCaptialVecPtr_;
    MA maSimpleStrategyCapital250Tracker_;
    double maxSimpleStrategyCapital_;
    double minBacktrackCross_ = 0.0;
    bool addLot2Trigerred = false;
};

#endif // STRATEGYTIEKUANGSHISIMPLEEX_H
