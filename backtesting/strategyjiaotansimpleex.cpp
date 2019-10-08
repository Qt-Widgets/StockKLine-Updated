#include "strategyjiaotansimpleex.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyJiaoTanSimpleEx::StrategyJiaoTanSimpleEx(TradeGatewayPtr pTradeGatewayforSimple)
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    captialVecPtr_(std::make_shared<std::vector<double>>()),
    simpleStrategyCaptialVecPtr_(std::make_shared<std::vector<double>>()),
    lastD_(0.0), currentD_(0.0), lastK_(0.0), currentK_(0.0),
    lastDiff1_(0.0), currentDiff1_(0.0),
    maClose10Tracker(10, closeVecPtr_),
    maClose17Tracker(17, closeVecPtr_),
    maClose200Tracker(200, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    windowedMaxMin_(60), lastRsv_(0.0),
    maCapitalTracker_(250, captialVecPtr_),
    maSimpleStrategyCapital250Tracker_(250, simpleStrategyCaptialVecPtr_),
    tradeGatewayForSimplePtr_(pTradeGatewayforSimple),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyJiaoTanSimpleEx::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyJiaoTanSimpleEx::onTick(Tick &tick)
{

}

int StrategyJiaoTanSimpleEx::min(int a, int b)
{
    return a < b ? a : b;
}

void StrategyJiaoTanSimpleEx::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    windowedMaxMin_.update(close);
    double maClose250 = maClose250Tracker.value();
    double maClose200 = maClose200Tracker.value();
    double maClose17 = maClose17Tracker.value();
    double maClose10 = maClose10Tracker.value();
    double diff = maClose17 - maClose200;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    double diff1 = maClose10 - maClose250;
    lastDiff1_ = currentDiff1_;
    currentDiff1_ = diff1;

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

    if (diff1 < 100.0 && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ > 0.0 && currentDiff_ < 0.0) ||
              (lastDiff_ > 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ < 0.0)
            ) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (close > maClose250 &&
             ((lastDiff1_ > 270.0 && currentDiff1_ < 270.0) ||
              (lastDiff1_ > 270.0 && currentDiff1_ == 270.0) ||
              (lastDiff1_ == 270.0 && currentDiff1_ < 270.0)
              )
             ) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > -100.0 && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ < 0.0 && currentDiff_ > 0.0) ||
              (lastDiff_ < 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ > 0.0)
            ) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (close < maClose250 &&
             ((lastDiff1_ < -270.0 && currentDiff1_ > -270.0) ||
              (lastDiff1_ < -270.0 && currentDiff1_ == -270.0) ||
              (lastDiff1_ == -270.0 && currentDiff1_ > -270.0)
              )
             ) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
}

bool StrategyJiaoTanSimpleEx::crossDK()
{
    return (lastD_ < lastK_ && currentD_ > currentK_) ||
            (lastD_ == lastK_ && currentD_ > currentK_) ||
            (lastD_ < lastK_ && currentD_ == currentK_);
}

bool StrategyJiaoTanSimpleEx::crossKD()
{
    return (lastK_ < lastD_ && currentK_ > currentD_) ||
            (lastK_ == lastD_ && currentK_ > currentD_) ||
            (lastK_ < lastD_ && currentK_ == currentD_);
}

void StrategyJiaoTanSimpleEx::openPosition(int volume)
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

void StrategyJiaoTanSimpleEx::closePosition(int volume)
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

void StrategyJiaoTanSimpleEx::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyJiaoTanSimpleEx::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyJiaoTanSimpleEx::getCloseMinute(KLineDataType &bar)
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

void StrategyJiaoTanSimpleEx::onTrade(Trade &trade)
{

}

void StrategyJiaoTanSimpleEx::onOrder(OrderRsp &order)
{

}

std::string StrategyJiaoTanSimpleEx::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyJiaoTanSimpleEx::instrumentType()
{
    return "i";
}
