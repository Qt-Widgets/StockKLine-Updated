#ifndef BACKTESTINGDRIVERFORJIAOTANSIMPLEEX_H
#define BACKTESTINGDRIVERFORJIAOTANSIMPLEEX_H

#include <vector>
#include <memory>
#include "KlineData.h"

class BacktestingDriverForJiaoTanSimpleEx
{
public:
    void test();

private:
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(std::vector<std::string>& files);
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(const std::string &instrumentID, std::string filePath);
    void split(const std::string& s, char c, std::vector<std::string>& v);
};

using KLineVecPtr = std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>>;

#endif // BACKTESTINGDRIVERFORJIAOTANSIMPLEEX_H
