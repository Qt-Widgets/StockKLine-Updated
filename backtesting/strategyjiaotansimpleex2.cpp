#include "strategyjiaotansimpleex2.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyJiaoTanSimpleEx2::StrategyJiaoTanSimpleEx2(TradeGatewayPtr pTradeGatewayforSimple)
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    captialVecPtr_(std::make_shared<std::vector<double>>()),
    simpleStrategyCaptialVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff1_(0.0), currentDiff1_(0.0),
    maClose10Tracker(10, closeVecPtr_),
    maClose17Tracker(17, closeVecPtr_),
    maClose15Tracker(15, closeVecPtr_),
    maClose50Tracker(50, closeVecPtr_),
    maClose170Tracker(170, closeVecPtr_),
    maClose190Tracker(190, closeVecPtr_),
    maCapitalTracker_(250, captialVecPtr_),
    maSimpleStrategyCapital250Tracker_(250, simpleStrategyCaptialVecPtr_),
    tradeGatewayForSimplePtr_(pTradeGatewayforSimple),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyJiaoTanSimpleEx2::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyJiaoTanSimpleEx2::onTick(Tick &tick)
{

}

int StrategyJiaoTanSimpleEx2::min(int a, int b)
{
    return a < b ? a : b;
}

void StrategyJiaoTanSimpleEx2::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    double maClose190 = maClose190Tracker.value();
    double maClose170 = maClose170Tracker.value();
    double maClose50 = maClose50Tracker.value();
    double maClose15 = maClose15Tracker.value();
    double maClose17 = maClose17Tracker.value();
    double maClose10 = maClose10Tracker.value();
    double diff = maClose15 - maClose170;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    double diff1 = maClose10 - maClose190;
    lastDiff1_ = currentDiff1_;
    currentDiff1_ = diff1;
    lastMA15_ = currentMA15_;
    currentMA15_ = maClose15;
    lastMA50_ = currentMA50_;
    currentMA50_ = maClose50;

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
            if (backtrack >= 0.0) {
                addLot2Trigerred = false;
                addLot3Trigerred = false;
            }

            if (maxPosCross_ < backtestingConfig->decLotDiffThreshold2 && capitalDiff > backtestingConfig->decLotDiffThreshold2) {
                closeAllPosition();
                maxPosCross_ = backtestingConfig->decLotDiffThreshold2 + 0.00000001;
                gateWay->recordAdjustmentSignal("Q");
            }
            else if (maxPosCross_ < backtestingConfig->decLotDiffThreshold1 && capitalDiff > backtestingConfig->decLotDiffThreshold1) {
                int volumeToCut = min(adjVolume_, backtestingConfig->baseLot);
                adjVolume_ -= volumeToCut;
                maxPosCross_ = backtestingConfig->decLotDiffThreshold1 + 0.00000001;
                gateWay->recordAdjustmentSignal("J");
            }

            // 清仓后，停止开仓，直到diff回到0轴
            if (maxPosCross_ >= backtestingConfig->decLotDiffThreshold2 && capitalDiff >= 0.0) {
                return;
            }

            if (!addLot3Trigerred &&
                    capitalDiff < backtestingConfig->addLotDiffThreshold1 &&
                    backtrack < backtestingConfig->addLotBacktrackThreshold2) {
                adjVolume_ = 3 * backtestingConfig->baseLot;
                gateWay->recordAdjustmentSignal("3");
                addLot3Trigerred = true;
            }
            else if (!addLot2Trigerred && !addLot3Trigerred &&
                     ((capitalDiff < backtestingConfig->addLotDiffThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1) ||
                      (backtrack < backtestingConfig->addLotBacktrackThreshold2)
                     )
               ) {
                adjVolume_ = 2 * backtestingConfig->baseLot;
                gateWay->recordAdjustmentSignal("2");
                addLot2Trigerred = true;
            }
        }
    }

    if (diff1 < 50.0 && close > maClose190 && diff > 0.0) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ > 0.0 && currentDiff_ < 0.0) ||
              (lastDiff_ > 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ < 0.0)
            ) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > 70.0 &&
             ((lastMA50_ < lastMA15_ && currentMA50_ > currentMA15_) ||
              (lastMA50_ < lastMA15_ && currentMA50_ == currentMA15_) ||
              (lastMA50_ == lastMA15_ && currentMA50_ > currentMA15_)
              )
             ) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > 400.0) {
        SP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 > -50.0 && close < maClose190 && diff < 0.0) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ < 0.0 && currentDiff_ > 0.0) ||
              (lastDiff_ < 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ > 0.0)
            ) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 < -70.0 &&
             ((lastMA15_ < lastMA50_ && currentMA15_ > currentMA50_) ||
              (lastMA15_ < lastMA50_ && currentMA15_ == currentMA50_) ||
              (lastMA15_ == lastMA50_ && currentMA15_ > currentMA50_)
              )
             ) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
    else if (diff1 < -400.0) {
        BP(backtestingConfig->baseLot + adjVolume_);
    }
}

void StrategyJiaoTanSimpleEx2::openPosition(int volume)
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

void StrategyJiaoTanSimpleEx2::closePosition(int volume)
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

void StrategyJiaoTanSimpleEx2::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyJiaoTanSimpleEx2::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx2::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx2::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx2::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx2::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyJiaoTanSimpleEx2::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyJiaoTanSimpleEx2::getCloseMinute(KLineDataType &bar)
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

void StrategyJiaoTanSimpleEx2::onTrade(Trade &trade)
{

}

void StrategyJiaoTanSimpleEx2::onOrder(OrderRsp &order)
{

}

std::string StrategyJiaoTanSimpleEx2::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyJiaoTanSimpleEx2::instrumentType()
{
    return "i";
}
