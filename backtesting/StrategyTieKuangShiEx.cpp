#include "StrategyTieKuangShiEx.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiEx::StrategyTieKuangShiEx()
	: closeVecPtr_(std::make_shared<std::vector<double>>()),
	captialVecPtr_(std::make_shared<std::vector<double>>()),
	lastDiff_(0.0), currentDiff_(0.0),
	maClose26Tracker(26, closeVecPtr_),
	maClose10Tracker(10, closeVecPtr_),
	maClose250Tracker(250, closeVecPtr_),
    maCapital250Tracker_(250, captialVecPtr_),
    backtestingConfig(BacktestingConfig::instance())
{

}

void StrategyTieKuangShiEx::init(TradeGatewayPtr pTradeGateway)
{
	this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiEx::onTick(Tick &tick)
{

}

void StrategyTieKuangShiEx::onBar(KLineDataType &bar)
{
	double close = bar.close_price;
	closeVecPtr_->push_back(close);
	double maClose250 = maClose250Tracker.value();
	double maClose26 = maClose26Tracker.value();
	double maClose10 = maClose10Tracker.value();
	int closeMinute = getCloseMinute(bar);
	double diff = maClose10 - maClose26;
	lastDiff_ = currentDiff_;
	currentDiff_ = diff;

	if (closeVecPtr_->size() <= 250) {
		return;
	}

	double capital = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_)->getAssetVaue();
	captialVecPtr_->push_back(capital);
	double maCaptial250 = maCapital250Tracker_.value();
	double capitalDiff = capital - maCaptial250;

	// 资金曲线250均线采集满，才开始应用均线调整策略
    if (closeVecPtr_->size() > 250 + backtestingConfig->capitalPeriod) {

		if (capitalDiff <= 0.0) {
			maxPosCross_ = 0.0;
		}
		else {
			minNegCross_ = 0.0;
		}

		int volumeToDecrease = 0;
        if (maxPosCross_ < backtestingConfig->posThreshold1 && capitalDiff > backtestingConfig->posThreshold1) {
			if (adjVolume_ >= 1) {
				volumeToDecrease++;
				adjVolume_--;
                maxPosCross_ = backtestingConfig->posThreshold1 + 0.1;
			}
		}
        if (maxPosCross_ < backtestingConfig->posThreshold2 && capitalDiff > backtestingConfig->posThreshold2) {
			if (adjVolume_ >= 1) {
				volumeToDecrease++;
				adjVolume_--;
                maxPosCross_ = backtestingConfig->posThreshold2 + 0.1;
			}
		}
        if (maxPosCross_ < backtestingConfig->posThreshold3 && capitalDiff > backtestingConfig->posThreshold3) {
			if (adjVolume_ >= 1) {
				volumeToDecrease++;
				adjVolume_--;
                maxPosCross_ = backtestingConfig->posThreshold3 + 0.1;
			}
		}
        if (maxPosCross_ < backtestingConfig->posThreshold4 && capitalDiff > backtestingConfig->posThreshold4) {
			if (adjVolume_ >= 1) {
				volumeToDecrease++;
				adjVolume_--;
                maxPosCross_ = backtestingConfig->posThreshold4 + 0.1;
			}
		}
        if (maxPosCross_ < backtestingConfig->posThreshold5 && capitalDiff > backtestingConfig->posThreshold5) {
			closeAllPosition();
            maxPosCross_ = backtestingConfig->posThreshold5 + 0.1;
		}
		if (volumeToDecrease > 0) {
			closePosition(volumeToDecrease);
		}

		// 9200清仓后，停止开仓，直到diff回到0轴
        if (maxPosCross_ >= backtestingConfig->posThreshold5 && capitalDiff >= 0.0) {
			return;
		}

		int volumeToIncrease = 0;
        if (minNegCross_ > backtestingConfig->negThreshold1 && capitalDiff < backtestingConfig->negThreshold1) {
			if (adjVolume_ + 2 <= 4) {
				volumeToIncrease += 2;
				adjVolume_ += 2;
                minNegCross_ = backtestingConfig->negThreshold1 - 0.1;
			}
		}
        if (minNegCross_ > backtestingConfig->negThreshold2 && capitalDiff < backtestingConfig->negThreshold2) {
			if (adjVolume_ + 1 <= 4) {
				volumeToIncrease++;
				adjVolume_++;
                minNegCross_ = backtestingConfig->negThreshold2 - 0.1;
			}
		}
        if (minNegCross_ > backtestingConfig->negThreshold3 && capitalDiff < backtestingConfig->negThreshold3) {
			if (adjVolume_ + 1 <= 4) {
				volumeToIncrease++;
				adjVolume_++;
                minNegCross_ = backtestingConfig->negThreshold3 - 0.1;
			}
		}
		openPosition(volumeToIncrease);
	}

	if (close > maClose250 && diff > 0.0) {
		BPK(baselVolume_ + adjVolume_);
	}
	else if (lastDiff_ > 0.0 && currentDiff_ < 0.0) {
		SP(baselVolume_ + adjVolume_);
	}
	else if (close < maClose250 && diff < 0.0) {
		SPK(baselVolume_ + adjVolume_);
	}
	else if (lastDiff_ < 0.0 && currentDiff_ > 0.0) {
		BP(baselVolume_ + adjVolume_);
	}
}

void StrategyTieKuangShiEx::openPosition(int volume)
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

void StrategyTieKuangShiEx::closePosition(int volume)
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

void StrategyTieKuangShiEx::closeAllPosition()
{
	closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiEx::BPK(int volume)
{
	tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiEx::SPK(int volume)
{
	tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::SP(int volume)
{
	tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiEx::BP(int volume)
{
	tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::SK(int volume)
{
	tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::BK(int volume)
{
	tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiEx::getCloseMinute(KLineDataType &bar)
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

void StrategyTieKuangShiEx::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiEx::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiEx::name()
{
	return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiEx::instrumentType()
{
	return "i";
}
