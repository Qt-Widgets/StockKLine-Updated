#ifndef DATAFILE_H
#define DATAFILE_H

#include <vector>
#include <QString>
#include <QFile>
#include <map>

struct KLine
{
    QString time;                   //时间
    double openingPrice;            //开盘
    double highestBid;              //最高
    double lowestBid;               //最低
    double closeingPrice;           //收盘
    double amountOfIncrease;        //涨幅
    double amountOfAmplitude;       //振幅
    QString totalVolume;            //总手
    QString totalAmount;            //金额
    double turnoverRate;            //换手率
    double volumeAmount;            //成交次数
    double averageLine5;            //5日均线
    double averageLine10;           //10日均线
    double averageLine20;           //20日均线
    double averageLine30;           //30日均线
    double averageLine60;           //60日均线
    double ftotalVolume;
    double ftotalAmount;
    double volumeAverage5;          //成交量5日均值
    double volumeAverage10;         //成交量10日均值

    double averages[6];
    QString tradingSignal;
    QString adjustmentSignal;

    double capital;
    double capitalAvgs[6];
    double capitalAvgDiff;

    double capitalForSimpleStrategy;
    double capitalAvgsForSimpleStrategy[6];
    double capitalAvgDiffForSimpleStrategy;
    double capitalBacktrackForSimpleStrategy;
    QString tradingSignalForSimpleStrategy;
};

class DataFile
{
public:
    explicit DataFile();
    ~DataFile();
    bool readData(QString filestr);
    bool readBacktestingResult(QString filestr);
    bool readBacktestingSimpleResult(QString filestr);
    void clear();
    std::vector<KLine> kline;

    void calAverageLine();
    void calCapitalAverageLine();
    void calCapitalSimpleAverageLine();

    void calvolumeAverage5();
    void calvolumeAverage10();

    constexpr static int averageLineCount = 4;
    int averageLinePeriod[averageLineCount] = {15, 50, 170, 190};
    Qt::GlobalColor averageLineColors[averageLineCount] = {Qt::white, Qt::yellow, Qt::magenta};

    constexpr static int capitalAverageLineCount = 1;
    int capitalAverageLinePeriod[capitalAverageLineCount] = {250};
    Qt::GlobalColor capitalAverageLineColors[capitalAverageLineCount] = {Qt::magenta};

    std::map<QString, Qt::GlobalColor> tradingSignalColors = {
        {"BPK", Qt::yellow}, {"SPK", Qt::red} , {"BK", Qt::white},
        {"SK", Qt::green}, {"BP", Qt::blue}, {"SP", Qt::darkMagenta}
    };

private:
    void split(const std::string& s, char c, std::vector<std::string>& v);
};

#endif // DATAFILE_H
