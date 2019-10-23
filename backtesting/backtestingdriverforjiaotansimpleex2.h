#ifndef BACKTESTINGDRIVERFORJIAOTANSIMPLEEX2_H
#define BACKTESTINGDRIVERFORJIAOTANSIMPLEEX2_H

#include <vector>
#include <memory>
#include "KlineData.h"
#include "backtestingdriverinterface.h"

class BacktestingDriverForJiaoTanSimpleEx2 : public BacktestingDriverInterface
{
public:
    void test();

private:
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(std::vector<std::string>& files);
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(const std::string &instrumentID, std::string filePath);
    void split(const std::string& s, char c, std::vector<std::string>& v);
};

using KLineVecPtr = std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>>;

#endif // BACKTESTINGDRIVERFORJIAOTANSIMPLEEX2_H
