#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <deque>
#include <iostream>
#include "BacktestingTradeGateway.h"
#include "ThostFtdcUserApiDataType.h"

BacktestingTradeGateway::BacktestingTradeGateway(std::shared_ptr<EventEngine> eventEngine, std::string statFileName)
	: stopOrderCount_(0), limitOrderCount_(0), tradeCount_(0), capital_(1000000.0), slippage_(0.0), rate_(0.0),
	size_(0.0), priceTick_(0.0), position_(0), marketDataType_(MarketDataType::BarData),
	eventEngine_(eventEngine), statFileName_(statFileName)
{
}

void BacktestingTradeGateway::reset()
{
	capital_ = 1000000.0;
	position_ = 0;
	positionValue_ = 0.0;
	tradeCount_ = 0;
	limitOrderCount_ = 0;
	stopOrderCount_ = 0;
	limitOrders_.clear();
	stopOrders_.clear();
	longTrades_.clear();
	shortTrades_.clear();
	workingLimitOrders_.clear();
	workingStopOrders_.clear();
}

BacktestingTradeGateway::~BacktestingTradeGateway()
{
	if (statFilePtr_) {
		statFilePtr_->close();
	}
}

bool BacktestingTradeGateway::init()
{
	statFilePtr_ = std::make_unique<std::ofstream>();
	statFilePtr_->open(statFileName_, std::ios::out);

	if (!statFilePtr_->is_open()) {
		std::cout << "Cannot open file: " << statFileName_ << std::endl;
		return false;
	}
	return true;
}

void BacktestingTradeGateway::recordStatus()
{
	(*statFilePtr_) << bar_.update_time << "," 
		<< lastTradingSignal_ << ","
		<< capital_ + positionValue_ << ","
        << adjustmentSignal_ << ","
        << getLongTradeVolume() << ","
        << getShortTradeVolume()
		<< std::endl;
}

int BacktestingTradeGateway::getLongTradeVolume() {
    int volume = 0;
    for (auto& trade : longTrades_) {
        volume += trade->Volume;
    }
    return volume;
}

int BacktestingTradeGateway::getShortTradeVolume() {
    int volume = 0;
    for (auto& trade : shortTrades_) {
        volume += trade->Volume;
    }
    return volume;
}

// BK
void BacktestingTradeGateway::openLong(const char *instrumentID, int volume)
{
    if (longTrades_.size() != 0 || shortTrades_.size() != 0) {
		std::cout << "多头持仓为 " << shortTrades_.size() << "，BK不执行" << std::endl;
		return;
	}
	sendOrder(instrumentID, std::numeric_limits<double>::max(), 100 * volume, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);
	lastTradingSignal_ = "BK";
}

// SK
void BacktestingTradeGateway::openShort(const char *instrumentID, int volume)
{
    if (shortTrades_.size() != 0 || longTrades_.size() != 0) {
		std::cout << "空头持仓为 " << longTrades_.size() << "，SK不执行" << std::endl;
		return;
	}
	sendOrder(instrumentID, std::numeric_limits<double>::min(), 100 * volume, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open);
	lastTradingSignal_ = "SK";
}

// SP
void BacktestingTradeGateway::closeLong(const char *instrumentID, int volume)
{
	if (longTrades_.size() == 0) {
		std::cout << "多头持仓为 0，SP不执行" << std::endl;
		return;
	}
    //sendOrder(instrumentID, std::numeric_limits<double>::min(), 100 * volume, THOST_FTDC_D_Sell, THOST_FTDC_OF_Close);
    sendOrder(instrumentID, std::numeric_limits<double>::min(), getLongTradeVolume(), THOST_FTDC_D_Sell, THOST_FTDC_OF_Close);
	lastTradingSignal_ = "SP";
}

// BP
void BacktestingTradeGateway::closeShort(const char *instrumentID, int volume)
{
	if (shortTrades_.size() == 0) {
		std::cout << "空头持仓为 0，BP不执行" << std::endl;
		return;
	}
    //sendOrder(instrumentID, std::numeric_limits<double>::max(), 100 * volume, THOST_FTDC_D_Buy, THOST_FTDC_OF_Close);
    sendOrder(instrumentID, std::numeric_limits<double>::max(), getShortTradeVolume(), THOST_FTDC_D_Buy, THOST_FTDC_OF_Close);
	lastTradingSignal_ = "BP";
}

// BPK
void BacktestingTradeGateway::closeShortAndOpenLong(const char *instrumentID, int volume)
{
	int shortPositions = shortTrades_.size();
	if (shortPositions != 0) {
        //sendOrder(instrumentID, std::numeric_limits<double>::max(), shortTrades_.size(), THOST_FTDC_D_Buy, THOST_FTDC_OF_Close);
        sendOrder(instrumentID, std::numeric_limits<double>::max(), getShortTradeVolume(), THOST_FTDC_D_Buy, THOST_FTDC_OF_Close);
	}
	if (longTrades_.size() == 0) {
        sendOrder(instrumentID, std::numeric_limits<double>::max(), 100 * volume, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);
		lastTradingSignal_ = "BPK";
	}
}

// SPK
void BacktestingTradeGateway::closeLongAndOpenShort(const char *instrumentID, int volume)
{
	int longPositions = longTrades_.size();
	if (longPositions != 0) {
        //sendOrder(instrumentID, std::numeric_limits<double>::min(), longTrades_.size(), THOST_FTDC_D_Sell, THOST_FTDC_OF_Close);
        sendOrder(instrumentID, std::numeric_limits<double>::min(), getLongTradeVolume(), THOST_FTDC_D_Sell, THOST_FTDC_OF_Close);
	}
	if (shortTrades_.size() == 0) {
		sendOrder(instrumentID, std::numeric_limits<double>::min(), 100 * volume, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open);
		lastTradingSignal_ = "SPK";
	}
}

/*
void BacktestingTradeGateway::newBar(KLineDataType& bar)
{
	//bar_ = bar;
	strncpy(tradingDate_, bar.update_time, 8);
	tradingDate_[9] = '\0';
	strncpy(updateTime_, bar.update_time + 9, 9);

	crossLimitOrder(bar);
	strategyPtr_->onBar(bar);
}

void BacktestingTradeGateway::newTick(Tick& tick)
{
	//tick_ = tick;
	strcpy(tradingDate_, tick.tradingDate);
	strcpy(updateTime_, tick.updateTime);

	crossLimitOrder(tick);
	strategyPtr_->onTick(tick);
}
*/

void BacktestingTradeGateway::recordAdjustmentSignal(std::string signal)
{
    adjustmentSignal_ = signal;
}

void BacktestingTradeGateway::newMarketDataEvent(Event& event)
{
	switch (event.type) {
		case EventType::BarEvent:
			strncpy(tradingDate_, event.data.kline.update_time, 10);
			tradingDate_[10] = '\0';
			strcpy(updateTime_, event.data.kline.update_time + 11);

			bar_ = event.data.kline;
			crossLimitOrder(event);
			calculatePositionValue(bar_.close_price);
			lastTradingSignal_ = "";
			//strategyPtr_->onBar(event.data.kline);
			break;
		case EventType::TickEvent:
			strcpy(tradingDate_, event.data.tick.tradingDate);
			strcpy(updateTime_, event.data.tick.updateTime);

			tick_ = event.data.tick;
			crossLimitOrder(event);
			calculatePositionValue(tick_.lastPrice);
			lastTradingSignal_ = "";
			//strategyPtr_->onTick(event.data.tick);
			break;
	}
}

void BacktestingTradeGateway::cancelOrder(const std::string& orderID)
{
	auto iter = workingLimitOrders_.find(orderID);
	if (iter != workingLimitOrders_.end()) {
		OrderRspPtr orderRspPtr = iter->second;
		orderRspPtr->OrderStatus = THOST_FTDC_OST_Canceled;
		strcpy(orderRspPtr->CancelTime, updateTime_);
		//strategyPtr_->onOrder(*orderRspPtr);
		workingLimitOrders_.erase(iter);
	}
}

std::string BacktestingTradeGateway::sendOrder(
	const char *instrumentID,
	double price,
	int volume,
	char buySell,
	char openClose,
	char priceType)
{
	//不需要创建Order,因为回调需要OrderRsp
	limitOrderCount_++;
	OrderRspPtr orderRspPtr = std::make_shared<OrderRsp>();
	orderRspPtr->LimitPrice = price;
	orderRspPtr->VolumeTotalOriginal = volume;
	sprintf(orderRspPtr->OrderRef, "%d", limitOrderCount_);
	strcpy(orderRspPtr->InsertDate, tradingDate_);
	strcpy(orderRspPtr->InsertTime,  updateTime_);
	orderRspPtr->Direction = buySell;
	orderRspPtr->CombOffsetFlag[0] = openClose;
	orderRspPtr->OrderPriceType = priceType;

	workingLimitOrders_.emplace(orderRspPtr->OrderRef, orderRspPtr);
	limitOrders_.emplace(orderRspPtr->OrderRef, orderRspPtr);

	return orderRspPtr->OrderRef;
}

void BacktestingTradeGateway::crossLimitOrder(Event& event)
{
	double buyCrossPrice;
	double sellCrossPrice;
	double buyBestCrossPrice;
	double sellBestCrossPrice;

	//先确定会撮合成交的价格
	if (event.type == EventType::BarEvent) {
		KLineDataType& bar = event.data.kline;
		buyCrossPrice = bar.low_price;			//若买入方向限价单价格高于该价格，则会成交
		sellCrossPrice = bar.high_price;		//若卖出方向限价单价格低于该价格，则会成交
		buyBestCrossPrice = bar.open_price;	//在当前时间点前发出的买入委托可能的最优成交价
		sellBestCrossPrice = bar.open_price;	//在当前时间点前发出的卖出委托可能的最优成交价
	}
	else {
		Tick& tick = event.data.tick;
		buyCrossPrice = tick.lastPrice;
		sellCrossPrice = tick.lastPrice;
		buyBestCrossPrice = tick.lastPrice;
		sellBestCrossPrice = tick.lastPrice;
	}

	auto it = workingLimitOrders_.begin();
	while (it != workingLimitOrders_.end()) {
		OrderRspPtr orderRspPtr = it->second;

		//推送委托进入队列（未成交）的状态更新
		if (orderRspPtr->OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing) {
			//strategyPtr_->onOrder(*orderRspPtr);
		}

		//判断是否会成交
		bool buyCross = orderRspPtr->Direction == THOST_FTDC_D_Buy && 
			orderRspPtr->LimitPrice >= buyBestCrossPrice && buyCrossPrice > 0.0;	//国内的tick行情在涨停时askPrice1为0，此时买无法成交
		bool sellCross = orderRspPtr->Direction == THOST_FTDC_D_Sell &&
			orderRspPtr->LimitPrice <= sellBestCrossPrice && sellCrossPrice > 0.0;

		if (buyCross || sellCross) {
			tradeCount_++;
			TradePtr tradePtr = std::make_shared<Trade>();
			strcpy(tradePtr->InstrumentID, orderRspPtr->InstrumentID);
			sprintf(tradePtr->TradeID, "%d", tradeCount_);
			strcpy(tradePtr->OrderRef, orderRspPtr->OrderRef);
			tradePtr->Direction = orderRspPtr->Direction;
			tradePtr->OffsetFlag = orderRspPtr->CombOffsetFlag[0];

			//以买入为例：
			//1. 假设当根K线的OHLC分别为：100, 125, 90, 110
			//2. 假设在上一根K线结束(也是当前K线开始)的时刻，策略发出的委托为限价105
			//3. 则在实际中的成交价会是100而不是105，因为委托发出时市场的最优价格是100
			if (buyCross) {
				tradePtr->Price = std::min(orderRspPtr->LimitPrice, buyBestCrossPrice);
				position_ += orderRspPtr->VolumeTotalOriginal;
			}
			else {
				tradePtr->Price = std::max(orderRspPtr->LimitPrice, sellBestCrossPrice);
				position_ -= orderRspPtr->VolumeTotalOriginal;
			}
			tradePtr->Volume = orderRspPtr->VolumeTotalOriginal;
			strcpy(tradePtr->TradeTime, tradingDate_);
			strcpy(tradePtr->TradingDay, updateTime_);
			trades_.emplace(tradePtr->TradeID, tradePtr);

			//strategyPtr_->onTrade(*tradePtr);

			orderRspPtr->VolumeTraded = orderRspPtr->VolumeTotalOriginal;
			orderRspPtr->OrderStatus = THOST_FTDC_OST_AllTraded;
			//strategyPtr_->onOrder(*orderRspPtr);

			//从字典中删除该限价单
			//workingLimitOrders_.erase(orderRspPtr->OrderRef);
			it = workingLimitOrders_.erase(it);

			calculateBacktestingResult(*tradePtr);

			std::cout << capital_ << "," << positionValue_ << std::endl;
		}
		else {
			++it;
		}
	}
}

void BacktestingTradeGateway::crossStopOrder(Event& event)
{
	double buyCrossPrice;
	double sellCrossPrice;
	double bestCrossPrice;

	//先确定会撮合成交的价格，这里和限价单规则相反
	if (event.type == EventType::BarEvent) {
		KLineDataType& bar = event.data.kline;
		buyCrossPrice = bar.high_price;		//若买入方向停止单价格低于该价格，则会成交
		sellCrossPrice = bar.low_price;		//若卖出方向限价单价格高于该价格，则会成交
		bestCrossPrice = bar.open_price;	//最优成交价，买入停止单不能低于，卖出停止单不能高于
	}
	else {
		Tick& tick = event.data.tick;
		buyCrossPrice = tick.lastPrice;
		sellCrossPrice = tick.lastPrice;
		bestCrossPrice = tick.lastPrice;
	}

	auto it = workingStopOrders_.begin();
	while (it != workingStopOrders_.end()) {
		OrderRspPtr stopOrderRspPtr = it->second;
		bool buyCross = stopOrderRspPtr->Direction == THOST_FTDC_D_Buy && stopOrderRspPtr->StopPrice <= buyCrossPrice;
		bool sellCross = stopOrderRspPtr->Direction == THOST_FTDC_D_Sell && stopOrderRspPtr->StopPrice >= sellCrossPrice;

		if (buyCross || sellCross) {
			//更新停止单状态，并从字典中删除该停止单
			//stopOrderRspPtr->OrderStatus = STOPORDER_TRIGGERED

			//推送成交数据
			tradeCount_++;
			TradePtr tradePtr = std::make_shared<Trade>();
			strcpy(tradePtr->InstrumentID, stopOrderRspPtr->InstrumentID);
			sprintf(tradePtr->TradeID, "%d", tradeCount_);

			if (buyCross) {
				position_ += stopOrderRspPtr->VolumeTotalOriginal;
				tradePtr->Price = std::max(bestCrossPrice, stopOrderRspPtr->StopPrice);
			}
			else {
				position_ -= stopOrderRspPtr->VolumeTotalOriginal;
				tradePtr->Price = std::min(bestCrossPrice, stopOrderRspPtr->StopPrice);
			}

			limitOrderCount_++;
			sprintf(tradePtr->OrderRef, "%d", limitOrderCount_);
			tradePtr->Direction = stopOrderRspPtr->Direction;
			tradePtr->OffsetFlag = stopOrderRspPtr->CombOffsetFlag[0];
			tradePtr->Volume = stopOrderRspPtr->VolumeTotalOriginal;
			strcpy(tradePtr->TradeTime, tradingDate_);
			strcpy(tradePtr->TradingDay, updateTime_);

			trades_.emplace(tradePtr->TradeID, tradePtr);

			//推送委托数据
			stopOrderRspPtr->OrderStatus = THOST_FTDC_OST_AllTraded;
			//strategyPtr_->onOrder(*stopOrderRspPtr);
			//strategyPtr_->onTrade(*tradePtr);

			limitOrders_.emplace(stopOrderRspPtr->OrderRef, stopOrderRspPtr);
			//workingStopOrders_.erase(stopOrderRspPtr->OrderRef);
			it = workingStopOrders_.erase(it);
			calculateBacktestingResult(*tradePtr);

			std::cout << capital_ << "," << positionValue_ << std::endl;
		}
		else {
			++it;
		}
	}
}

//好在trade应该不多。。。
void BacktestingTradeGateway::createOrderRsp(Order &order, OrderRsp &orderRsp)
{
	strcpy(orderRsp.AccountID, order.AccountID);
	strcpy(orderRsp.BrokerID, order.BrokerID);
	strcpy(orderRsp.ClientID, order.ClientID);
	strcpy(orderRsp.CombHedgeFlag, order.CombHedgeFlag);
	strcpy(orderRsp.CombOffsetFlag, order.CombOffsetFlag);
	orderRsp.ContingentCondition = order.ContingentCondition;
	orderRsp.Direction = order.Direction;
	strcpy(orderRsp.ExchangeID, order.ExchangeID);
	orderRsp.ForceCloseReason = order.ForceCloseReason;
	orderRsp.LimitPrice = order.LimitPrice;
	strcpy(orderRsp.OrderRef, order.OrderRef);
	orderRsp.OrderPriceType = order.OrderPriceType;
	orderRsp.RequestID = order.RequestID;
	orderRsp.StopPrice = order.StopPrice;
	orderRsp.VolumeTotalOriginal = order.VolumeTotalOriginal;
	orderRsp.MinVolume = order.MinVolume;
}

void BacktestingTradeGateway::calculateBacktestingResult(Trade &trade)
{
	//open position
	if (trade.OffsetFlag == THOST_FTDC_OF_Open) {
		if (trade.Direction == THOST_FTDC_D_Buy) {
			longTrades_.push_back(std::make_shared<Trade>(trade));
		}
		else {
			shortTrades_.push_back(std::make_shared<Trade>(trade));
		}
		capital_ -= trade.Price * trade.Volume;
	}
	//close position, 先开先平
	else {
		long tradeVolume = trade.Volume;
		if (trade.Direction == THOST_FTDC_D_Buy) {
			for (auto it = shortTrades_.begin(); it != shortTrades_.end();) {
				if (tradeVolume > (*it)->Volume) {
					tradeVolume -= (*it)->Volume;
					capital_ += (*it)->Price * (*it)->Volume + ((*it)->Price - trade.Price) * (*it)->Volume;
					it = shortTrades_.erase(it);
				}
                else if (tradeVolume < (*it)->Volume) {
					(*it)->Volume -= tradeVolume;
					capital_ += (*it)->Price * tradeVolume + ((*it)->Price - trade.Price) * tradeVolume;
					break;
				}
				else {
                    capital_ += (*it)->Price * tradeVolume + ((*it)->Price - trade.Price) * tradeVolume;
                    shortTrades_.erase(it);
					break;
				}
			}
		}
		else {
			for (auto it = longTrades_.begin(); it != longTrades_.end();) {
				if (tradeVolume > (*it)->Volume) {
					tradeVolume -= (*it)->Volume;
					capital_ += trade.Price * (*it)->Volume;
					it = longTrades_.erase(it);
				}
				else if (tradeVolume < (*it)->Volume) {
					(*it)->Volume -= tradeVolume;
					capital_ += trade.Price * tradeVolume;
					break;
				}
				else {
					capital_ += trade.Price * tradeVolume;
					longTrades_.erase(it);
					break;
				}
			}
		}
	}
}

void BacktestingTradeGateway::calculatePositionValue(double currentPrice)
{
	positionValue_ = 0.0;
	for (auto &trade : longTrades_) {
		positionValue_ += currentPrice * trade->Volume;
	}
	for (auto &trade : shortTrades_) {
		positionValue_ += trade->Price * trade->Volume + (trade->Price - currentPrice) * trade->Volume;
	}
}

double BacktestingTradeGateway::getAssetVaue()
{
	return capital_ + positionValue_;
}

int BacktestingTradeGateway::getShortTradeCount()
{
	return shortTrades_.size();
}

int BacktestingTradeGateway::getLongTradeCount()
{
	return longTrades_.size();
}
