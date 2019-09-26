#include "strategytiekuangshisimpleex2.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiSimpleEx2::StrategyTieKuangShiSimpleEx2(TradeGatewayPtr pTradeGatewayforSimple)
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    captialVecPtr_(std::make_shared<std::vector<double>>()),
    simpleStrategyCaptialVecPtr_(std::make_shared<std::vector<double>>()),
    lastD_(0.0), currentD_(0.0), lastK_(0.0), currentK_(0.0),
    maClose5Tracker(5, closeVecPtr_),
    maClose6Tracker(6, closeVecPtr_),
    maClose20Tracker(20, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    windowedMaxMin_(60), lastRsv_(0.0),
    maCapitalTracker_(250, captialVecPtr_),
    maSimpleStrategyCapital250Tracker_(250, simpleStrategyCaptialVecPtr_),
    tradeGatewayForSimplePtr_(pTradeGatewayforSimple),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyTieKuangShiSimpleEx2::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiSimpleEx2::onTick(Tick &tick)
{

}

int StrategyTieKuangShiSimpleEx2::min(int a, int b)
{
    return a < b ? a : b;
}

void StrategyTieKuangShiSimpleEx2::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    windowedMaxMin_.update(close);
    double maClose250 = maClose250Tracker.value();
    double maClose5 = maClose5Tracker.value();
    double maClose6 = maClose6Tracker.value();
    double maClose20 = maClose20Tracker.value();
    double diff = maClose6 - maClose20;
    double diff1 = maClose5 - maClose250;
    lastD_ = currentD_;
    lastK_ = currentK_;
    double rsv = (close - windowedMaxMin_.getMin()) / (windowedMaxMin_.getMax() - windowedMaxMin_.getMin()) * 100.0;
    currentK_ = lastRsv_ * (20 - 1) / 20 + rsv * 1 / 20;
    currentD_ = lastK_ * (60 - 1) / 60 + currentK_ * 1 / 60;
    lastRsv_ = rsv;

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

        // note: the benchmark is all about the simple strategy. it has nothing to do with the adjustment based strategy.
        double capitalDiff = simpleStrategyCapital - maSimpleStrategyCapital250;
        double backtrack = simpleStrategyCapital - maxSimpleStrategyCapital_;

        // 资金曲线250均线采集满，才开始应用均线调整策略
        if (closeVecPtr_->size() > 250 + backtestingConfig->capitalPeriod) {

            if (capitalDiff <= 0.0) {
                maxPosCross_ = 0.0;
            }
            else {
                minNegCross_ = 0.0;
                //addLot2Trigerred = false;
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

            // 清仓后，停止开仓，直到diff回到0轴
            if (maxPosCross_ >= backtestingConfig->decLotDiffThreshold2 && capitalDiff >= 0.0) {
                return;
            }

            int volumeToIncrease = 0;
            if (
                    (minNegCross_ > backtestingConfig->addLotDiffThreshold1 && capitalDiff < backtestingConfig->addLotDiffThreshold1)
                    && (minBacktrackCross_ > backtestingConfig->addLotBacktrackThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
               ) {
                volumeToIncrease += backtestingConfig->baseLot;
                adjVolume_ += backtestingConfig->baseLot;
                if (minNegCross_ > backtestingConfig->addLotDiffThreshold1 && capitalDiff < backtestingConfig->addLotDiffThreshold1)
                    minNegCross_ = backtestingConfig->addLotDiffThreshold1 - 0.00000001;
                if (minBacktrackCross_ > backtestingConfig->addLotBacktrackThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1)
                    minBacktrackCross_ = backtestingConfig->addLotBacktrackThreshold1 - 0.00000001;
                gateWay->recordAdjustmentSignal("2");
            }
            if (
                    (! addLot2Trigerred) &&
                    (backtrack < backtestingConfig->addLotBacktrackThreshold2)
               ) {
                volumeToIncrease += backtestingConfig->baseLot;
                adjVolume_ += backtestingConfig->baseLot;
                gateWay->recordAdjustmentSignal("3");
                addLot2Trigerred = true;
            }
        }
    }

    if (diff1 < 42 && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > -42 && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if (close > maClose250 && crossDK()) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > 42 && close > maClose250 && crossKD()) {
        BK(backtestingConfig->baseLot + adjVolume_);
    }
    else if (close < maClose250 && crossKD()) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 < -42 && close < maClose250 && crossDK()) {
        SK(backtestingConfig->baseLot + adjVolume_);
    }
}

bool StrategyTieKuangShiSimpleEx2::crossDK()
{
    return (lastD_ < lastK_ && currentD_ > currentK_) ||
            (lastD_ == lastK_ && currentD_ > currentK_) ||
            (lastD_ < lastK_ && currentD_ == currentK_);
}

bool StrategyTieKuangShiSimpleEx2::crossKD()
{
    return (lastK_ < lastD_ && currentK_ > currentD_) ||
            (lastK_ == lastD_ && currentK_ > currentD_) ||
            (lastK_ < lastD_ && currentK_ == currentD_);
}

void StrategyTieKuangShiSimpleEx2::openPosition(int volume)
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

void StrategyTieKuangShiSimpleEx2::closePosition(int volume)
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

void StrategyTieKuangShiSimpleEx2::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiSimpleEx2::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx2::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx2::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx2::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx2::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiSimpleEx2::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiSimpleEx2::getCloseMinute(KLineDataType &bar)
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

void StrategyTieKuangShiSimpleEx2::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiSimpleEx2::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiSimpleEx2::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiSimpleEx2::instrumentType()
{
    return "i";
}
