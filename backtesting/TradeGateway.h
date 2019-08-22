#ifndef TRADEGATEWAY_H
#define TRADEGATEWAY_H

#include <memory>
#include <string>
#include "ThostFtdcUserApiStruct.h"

using Order = CThostFtdcInputOrderField;
using OrderRsp = CThostFtdcOrderField;
using Trade = CThostFtdcTradeField;

class TradeGateway
{
public:
	// TO-DO: data types
	virtual std::string sendOrder(const char *instrumentID, double price, int volume, char buySell, char openClose, char priceType) = 0;
	virtual void cancelOrder(const std::string&) = 0;

	virtual void openLong(const char *instrumentID, int volume) = 0; //买入开仓
	virtual void openShort(const char *instrumentID, int volume) = 0; //卖出开仓
	virtual void closeLong(const char *instrumentID, int volume) = 0; //卖出平仓
	virtual void closeShort(const char *instrumentID, int volume) = 0; //买入平仓
	virtual void closeShortAndOpenLong(const char *instrumentID, int volume) = 0; //反手做多（先买入平仓再立即买入开仓）
	virtual void closeLongAndOpenShort(const char *instrumentID, int volume) = 0; //反手做空（先卖出平仓再立即卖出开仓）
};

using TradeGatewayPtr = std::shared_ptr<TradeGateway>;
using OrderPtr = std::shared_ptr<Order>;
using OrderRspPtr = std::shared_ptr<OrderRsp>;
using TradePtr = std::shared_ptr<Trade>;

#endif
