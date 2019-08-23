#include "datafile.h"
#include <vector>

DataFile::DataFile()
{

}

DataFile::~DataFile()
{

}

void DataFile::clear()
{
    kline.clear();
}

bool DataFile::readBacktestingResult(QString filestr)
{
    QFile file(filestr);
    if( !file.open(QFile::ReadOnly) )
        return false;

    char    line[1024];
    char    *token;
    QString time;
    QString tradingSignal;
    double capital = 0.0;;
    int klineIndex = 0;
    std::vector<std::string> columns;

    while( file.readLine(line,1024)  > 0 ) {
        columns.clear();
        split(line, ',', columns);

        kline[klineIndex].tradingSignal = columns[1].c_str();
        kline[klineIndex].capital = std::atof(columns[2].c_str());
        klineIndex++;
    }

    calCapitalAverageLine();
}

void DataFile::split(const std::string& s, char c, std::vector<std::string>& v)
{
    std::string::size_type i = 0;
    std::string::size_type j = s.find(c);

    while (j != std::string::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::string::npos)
            v.push_back(s.substr(i, s.length()));
    }
}

bool DataFile::readData(QString filestr)
{
    QFile file(filestr);
    if( !file.open(QFile::ReadOnly) )
        return false;

    char    line[1024];
    char    *token;
    KLine temp;

    while( file.readLine(line,1024)  > 0 )
    {
        token = strtok( line, "," );
        if( token != nullptr )
            temp.time = token;

        token = strtok( nullptr, "," );
        if( token != nullptr )
            temp.openingPrice = atof(token);

        token = strtok( nullptr, "," );
        if( token != nullptr )
            temp.highestBid = atof(token);

        token = strtok( nullptr, "," );
        if( token != nullptr )
            temp.lowestBid = atof(token);

        token = strtok( nullptr, "," );
        if( token != nullptr )
            temp.closeingPrice = atof(token);

        token = strtok( nullptr, "," );
        if( token != nullptr ) {
            temp.totalVolume = token;
            temp.ftotalVolume = atof(token);
        }

        kline.push_back(temp);
    }

    calAverageLine();
    return true;
}

void DataFile::calAverageLine()
{
    for (int i = 0; i < averageLineCount; i++) {
        double sum = 0.0;
        int period = averageLinePeriod[i];
        for (int j = 0; j < period - 1; j++) {
            sum += kline[j].closeingPrice;
            kline[j].averages[i] = 0.0;
        }
        sum += kline[period - 1].closeingPrice;
        kline[period - 1].averages[i] = sum / period;
        for (int j = period; j < kline.size(); j++) {
            sum += kline[j].closeingPrice;
            sum -= kline[j - period].closeingPrice;
            kline[j].averages[i] = sum / period;
        }
    }
}

void DataFile::calCapitalAverageLine()
{
    for (int i = 0; i < capitalAverageLineCount; i++) {
        double sum = 0.0;
        int period = capitalAverageLinePeriod[i];
        for (int j = 0; j < period - 1; j++) {
            sum += kline[j].capital;
            kline[j].capitalAvgs[i] = 0.0;
        }
        sum += kline[period - 1].capital;
        kline[period - 1].capitalAvgs[i] = sum / period;
        for (int j = period; j < kline.size(); j++) {
            sum += kline[j].capital;
            sum -= kline[j - period].capital;
            kline[j].capitalAvgs[i] = sum / period;
        }
    }

    for (int i = 0; i < kline.size(); ++i) {
        if (i < capitalAverageLinePeriod[0]) {
            kline[i].capitalAvgDiff = 0.0;
        } else {
            kline[i].capitalAvgDiff = kline[i].capital - kline[i].capitalAvgs[0];
        }
    }
}
