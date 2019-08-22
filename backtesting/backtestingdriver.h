#ifndef BACKTESTINGDRIVER_H
#define BACKTESTINGDRIVER_H

#include <vector>
#include <memory>
#include "KlineData.h"

class BacktestingDriver
{
public:
    void test();

private:
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> BacktestingDriver::loadKline(std::vector<std::string>& files);
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> BacktestingDriver::loadKline(const std::string &instrumentID, std::string filePath);
    void split(const std::string& s, char c, std::vector<std::string>& v);
};

using KLineVecPtr = std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>>;

#endif // BACKTESTINGDRIVER_H
