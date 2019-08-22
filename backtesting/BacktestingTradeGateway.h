#ifndef BACKTESTINGTRADEGATEWAY_H
#define BACKTESTINGTRADEGATEWAY_H

#include <unordered_map>
#include <string>
#include <fstream>
#include "TradeGateway.h"
#include "KlineData.h"
#include "Strategy.h"
#include "EventEngine.h"

// we need to simulate order cross
class BacktestingTradeGateway : public TradeGateway
{
public:
	BacktestingTradeGateway(std::shared_ptr<EventEngine> eventEngine, std::string statFileName);
	~BacktestingTradeGateway();
	bool init();
	std::string sendOrder(const char *instrumentID, double price, int volume, char buySell, char openClose, char priceType = THOST_FTDC_OPT_LimitPrice) override;
	void cancelOrder(const std::string &orderID) override;

	void openLong(const char *instrumentID, int volume) override;
	void openShort(const char *instrumentID, int volume) override;
	void closeLong(const char *instrumentID, int volume) override;
	void closeShort(const char *instrumentID, int volume) override;
	void closeShortAndOpenLong(const char *instrumentID, int volume) override;
	void closeLongAndOpenShort(const char *instrumentID, int volume) override;

	//void newBar(KLineDataType &bar);
	//void newTick(Tick &tick);
	void newMarketDataEvent(Event& event);
	void calculateBacktestingResult(Trade &trade);
	void recordStatus();
	double getAssetVaue();
	int getShortTradeCount();
	int getLongTradeCount();
	void reset();

private:
	void crossLimitOrder(Event& event);
	void crossStopOrder(Event& event);
	void calculatePositionValue(double currentPrice);

	void createOrderRsp(Order &order, OrderRsp &orderRsp);

private:
	StrategyPtr strategyPtr_;
	std::shared_ptr<EventEngine> eventEngine_;

	std::unordered_map<std::string, OrderRspPtr> stopOrders_;
	std::unordered_map<std::string, OrderRspPtr> workingStopOrders_;
	long stopOrderCount_;	//stopOrderID = STOPORDERPREFIX + str(stopOrderCount)

	std::unordered_map<std::string, OrderRspPtr> limitOrders_;
	std::unordered_map<std::string, OrderRspPtr> workingLimitOrders_;
	long limitOrderCount_;

	std::unordered_map<std::string, TradePtr> trades_;
	long tradeCount_;

	//持仓
	std::deque<TradePtr> longTrades_;
	std::deque<TradePtr> shortTrades_;

	double positionValue_ = 0.0;
	double capital_;				//回测时的起始本金
	double slippage_;				//回测时假设的滑点
	double rate_;					//回测时假设的佣金比例
	int size_;						//合约大小，默认为1 
	double priceTick_;				//价格最小变动
	int position_;					//持仓量

	Tick tick_;
	KLineDataType bar_;

	std::string lastTradingSignal_;

	//consistent with the CTP definition
	char tradingDate_[11];
	char updateTime_[9];

	MarketDataType marketDataType_;

	std::string statFileName_;
	std::unique_ptr<std::ofstream> statFilePtr_;
};

using BacktestingTradeGatewayPtr = std::shared_ptr<BacktestingTradeGateway>;

#endif
