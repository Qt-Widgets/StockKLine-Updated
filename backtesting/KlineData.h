#ifndef KLINEDATA_H
#define KLINEDATA_H

// k???????
struct KLineDataType
{
    char update_time[20];
    double open_price;   // ??
    double high_price;   // ??
    double low_price;    // ??
    double close_price;  // ??
    int volume;          // ??
    char instrumentID[31];
};

struct Tick {
    char tradingDate[9];
    char updateTime[9];
    char updateMillisec[9];
    double lastPrice;
    double volume;
    double openInterest;
    char actionDate[9];
    char instrumentID[31];
};

enum MarketDataType { TickData, BarData };

#endif
