#ifndef BACKTESTINGDRIVERFORLUOWENGANGSIMPLEEX_H
#define BACKTESTINGDRIVERFORLUOWENGANGSIMPLEEX_H

#include <vector>
#include <memory>
#include "KlineData.h"
#include "backtestingdriverinterface.h"

class BacktestingDriverForLuoWenGangSimpleEx : public BacktestingDriverInterface
{
public:
    void test();

private:
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(std::vector<std::string>& files);
    std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>> loadKline(const std::string &instrumentID, std::string filePath);
    void split(const std::string& s, char c, std::vector<std::string>& v);
};

using KLineVecPtr = std::unique_ptr<std::vector<std::shared_ptr<KLineDataType>>>;

#endif // BACKTESTINGDRIVERFORLUOWENGANGSIMPLEEX_H
