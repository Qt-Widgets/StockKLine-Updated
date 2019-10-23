#ifndef BACKTESTINGDRIVERINTERFACE_H
#define BACKTESTINGDRIVERINTERFACE_H

#include <vector>
#include <memory>
#include "KlineData.h"

class BacktestingDriverInterface
{
public:
    virtual void test() = 0;

private:
    virtual std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(std::vector<std::string>& files) = 0;
    virtual std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(const std::string &instrumentID, std::string filePath) = 0;
    virtual void split(const std::string& s, char c, std::vector<std::string>& v) = 0;
};

using KLineVecPtr = std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>>;

#endif // BACKTESTINGDRIVERINTERFACE_H
