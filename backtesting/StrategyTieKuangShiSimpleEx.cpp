#include "StrategyTieKuangShiSimpleEx.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiSimpleEx::StrategyTieKuangShiSimpleEx(TradeGatewayPtr pTradeGatewayforSimple)
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    captialVecPtr_(std::make_shared<std::vector<double>>()),
    simpleStrategyCaptialVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff_(0.0), currentDiff_(0.0),
    maClose26Tracker(26, closeVecPtr_),
    maClose10Tracker(10, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    maClose5Tracker(5, closeVecPtr_),
    maCapitalTracker_(250, captialVecPtr_),
    maSimpleStrategyCapital250Tracker_(250, simpleStrategyCaptialVecPtr_),
    tradeGatewayForSimplePtr_(pTradeGatewayforSimple),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyTieKuangShiSimpleEx::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiSimpleEx::onTick(Tick &tick)
{

}

int StrategyTieKuangShiSimpleEx::min(int a, int b)
{
    return a < b ? a : b;
}

void StrategyTieKuangShiSimpleEx::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    double maClose250 = maClose250Tracker.value();
    double maClose26 = maClose26Tracker.value();
    double maClose10 = maClose10Tracker.value();
    double maClose5 = maClose5Tracker.value();
    int closeMinute = getCloseMinute(bar);
    double diff = maClose10 - maClose26;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    double diff1 = maClose5 - maClose250;

    if (closeVecPtr_->size() <= 250) {
        return;
    }

    if (backtestingConfig->enableCapitalAjdustment) {

        auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
        // clear the signal each time
        gateWay->recordAdjustmentSignal("");

        // start calculate the MA capital line for the simple strategy
        double simpleStrategyCapital = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayForSimplePtr_)->getAssetVaue();
        simpleStrategyCaptialVecPtr_->push_back(simpleStrategyCapital);
        double maSimpleStrategyCapital250 = maSimpleStrategyCapital250Tracker_.value();
        maxSimpleStrategyCapital_ = std::fmax(maxSimpleStrategyCapital_, simpleStrategyCapital);

//        double capital = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_)->getAssetVaue();
//        captialVecPtr_->push_back(capital);
//        double maCapital250 = maCapitalTracker_.value();
//        double capitalDiff = capital - maCapital250;
//        double capitalDiffPercent = capitalDiff / maCapital250 * 100.0;

        // note: the benchmark is all about the simple strategy. it has nothing to do with the adjustment based strategy.
        double capitalDiff = simpleStrategyCapital - maSimpleStrategyCapital250;
        //double capitalDiffPercent = capitalDiff / maSimpleStrategyCapital250 * 100.0;
        double backtrack = simpleStrategyCapital - maxSimpleStrategyCapital_;
        //double backtrackPercent = backtrack / maxSimpleStrategyCapital_ * 100.0;

        // 资金曲线250均线采集满，才开始应用均线调整策略
        if (closeVecPtr_->size() > 250 + backtestingConfig->capitalPeriod) {

            if (capitalDiff <= 0.0) {
                maxPosCross_ = 0.0;
            }
            else {
                minNegCross_ = 0.0;
                addLot2Trigerred = false;
            }
            if (backtrack >= 0.0) {
                // so, there is no backtrack (only when the value is negtive)
                minBacktrackCross_ = 0.0;
                addLot2Trigerred = false;
            }

            int volumeToDecrease = 0;
            if (maxPosCross_ < backtestingConfig->decLotDiffThreshold1 && capitalDiff > backtestingConfig->decLotDiffThreshold1) {
                int volumeToCut = min(adjVolume_, backtestingConfig->baseLot);
                volumeToDecrease += volumeToCut;
                adjVolume_ -= volumeToCut;
                maxPosCross_ = backtestingConfig->decLotDiffThreshold1 + 0.00000001;
                gateWay->recordAdjustmentSignal("J");
            }
            else if (maxPosCross_ < backtestingConfig->decLotDiffThreshold2 && capitalDiff > backtestingConfig->decLotDiffThreshold2) {
                closeAllPosition();
                maxPosCross_ = backtestingConfig->decLotDiffThreshold2 + 0.00000001;
                gateWay->recordAdjustmentSignal("Q");
            }
//            if (volumeToDecrease > 0) {
//                closePosition(volumeToDecrease);
//            }

            // 清仓后，停止开仓，直到diff回到0轴
            if (maxPosCross_ >= backtestingConfig->decLotDiffThreshold2 && capitalDiff >= 0.0) {
                return;
            }

            int volumeToIncrease = 0;
            if (
                    (minNegCross_ > backtestingConfig->addLotDiffThreshold1 && capitalDiff < backtestingConfig->addLotDiffThreshold1)
                    || (minBacktrackCross_ > backtestingConfig->addLotBacktrackThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
               ) {
                volumeToIncrease += backtestingConfig->baseLot;
                adjVolume_ += backtestingConfig->baseLot;
                if (minNegCross_ > backtestingConfig->addLotDiffThreshold1 && capitalDiff < backtestingConfig->addLotDiffThreshold1)
                    minNegCross_ = backtestingConfig->addLotDiffThreshold1 - 0.00000001;
                if (minBacktrackCross_ > backtestingConfig->addLotBacktrackThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
                    minBacktrackCross_ = backtestingConfig->addLotBacktrackThreshold1 - 0.00000001;
                gateWay->recordAdjustmentSignal("2");
            }
//            if (
//                    (minNegCross_ > backtestingConfig->addLotDiffThreshold1 && capitalDiff < backtestingConfig->addLotDiffThreshold1)
//                    && (minBacktrackCross_ > backtestingConfig->addLotBacktrackThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
//               ) {
            if (
                    ! addLot2Trigerred &&
                    //(minNegCross_ < backtestingConfig->addLotDiffThreshold1 || minBacktrackCross_ < backtestingConfig->addLotBacktrackThreshold1) &&
                    (capitalDiff < backtestingConfig->addLotDiffThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
               ) {
                volumeToIncrease += backtestingConfig->baseLot;
                adjVolume_ += backtestingConfig->baseLot;
//                minNegCross_ = backtestingConfig->addLotDiffThreshold2 - 0.00000001;
//                minBacktrackCross_ = backtestingConfig->addLotBacktrackThreshold2 - 0.00000001;
                gateWay->recordAdjustmentSignal("3");
                addLot2Trigerred = true;
            }
//            openPosition(volumeToIncrease);
        }
    }

    if (diff1 < backtestingConfig->tieKuangShiN && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ > 0.0 && currentDiff_ < 0.0) ||
              (lastDiff_ > 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ < 0.0)
            ) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > -1.0 * backtestingConfig->tieKuangShiN && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ < 0.0 && currentDiff_ > 0.0) ||
              (lastDiff_ < 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ > 0.0)
            ) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
}

void StrategyTieKuangShiSimpleEx::openPosition(int volume)
{
    auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
    int longTradeCount = gateWay->getLongTradeCount();
    int shortTradeCount = gateWay->getShortTradeCount();
    if (longTradeCount > 0) {
        BK(volume);
    }
    else if (shortTradeCount > 0) {
        SK(volume);
    }
}

void StrategyTieKuangShiSimpleEx::closePosition(int volume)
{
    auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
    int longTradeCount = gateWay->getLongTradeCount();
    int shortTradeCount = gateWay->getShortTradeCount();
    if (longTradeCount > 0) {
        if (volume > longTradeCount) {
            SP(longTradeCount);
        }
        else {
            SP(volume);
        }
    }
    else if (shortTradeCount > 0) {
        if (volume > shortTradeCount) {
            BP(shortTradeCount);
        }
        else {
            BP(volume);
        }
    }
}

void StrategyTieKuangShiSimpleEx::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiSimpleEx::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiSimpleEx::getCloseMinute(KLineDataType &bar)
{
    int minutesOfDay = getMinutesOfDay(bar.update_time);

    int minutesToClose = (10 * 60 + 15) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (11 * 60 + 30) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (15 * 60) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (23 * 60 + 30) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    return minutesToClose;
}

void StrategyTieKuangShiSimpleEx::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiSimpleEx::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiSimpleEx::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiSimpleEx::instrumentType()
{
    return "i";
}
