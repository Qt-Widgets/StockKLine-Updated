#include "datafile.h"

DataFile::DataFile()
{

}


DataFile::~DataFile()
{

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
    double lastCapital = 1000000.0;
    double capital = 0.0;;
    int klineIndex = 0;

    while( file.readLine(line,1024)  > 0 ) {
        token = strtok( line, "," );
        if( token != nullptr )
            time = token;

        token = strtok( nullptr, "," );
        if( token != nullptr )
            tradingSignal = token;

        token = strtok( nullptr, "," );
        if( token != nullptr )
            capital = std::atof(token);

        while (kline[klineIndex].time != time) {
            kline[klineIndex].capital = lastCapital;
            klineIndex++;
        }
        kline[klineIndex].tradingSignal = tradingSignal;
        kline[klineIndex].capital = capital;
        lastCapital = capital;
        klineIndex++;
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
    //Corvert();
    //calvolumeAverage5();
    //calvolumeAverage10();
    return true;
}


void DataFile::calAverageLine()
{

//    // 初始化各均线的值
//    for(int i=0; i<4;i++)
//        kline[i].averageLine5 = 0;

//    for(int i=0;i<9;i++)
//        kline[i].averageLine10 = 0;

//    for(int i=0;i<19;i++)
//        kline[i].averageLine20 = 0;

//    for(int i=0;i<29;i++)
//        kline[i].averageLine30 = 0;

//    for(int i=0;i<59;i++)
//        kline[i].averageLine60 = 0;


//    calAverageLine5();
//    calAverageLine10();
//    calAverageLine20();
//    calAverageLine30();
//    calAverageLine60();

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


void DataFile::calAverageLine5()
{
    for( int i=4;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-4;j<=i;++j)
        {
            sum += kline[j].closeingPrice;
        }
        kline[i].averageLine5 = sum /5;
    }
}


void DataFile::calAverageLine10()
{
    for( int i=9;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-9;j<=i;++j)
        {
            sum += kline[j].closeingPrice;
        }
        kline[i].averageLine10 = sum /10;
    }
}




void DataFile::calAverageLine20()
{
    for( int i=19;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-19;j<=i;++j)
        {
            sum += kline[j].closeingPrice;
        }
        kline[i].averageLine20 = sum /20;
    }
}


void DataFile::calAverageLine30()
{
    for( int i=29;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-29;j<=i;++j)
        {
            sum += kline[j].closeingPrice;
        }
        kline[i].averageLine30 = sum /30;
    }
}



void DataFile::calAverageLine60()
{
    for( int i=59;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-59;j<=i;++j)
        {
            sum += kline[j].closeingPrice;
        }
        kline[i].averageLine60 = sum /60;
    }
}

void DataFile::calvolumeAverage5()
{

    for( int i=4;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-4;j<=i;++j)
        {
            sum += kline[j].ftotalVolume;
        }
        kline[i].volumeAverage5 = sum /5;
    }


}


void DataFile::calvolumeAverage10()
{
    for( int i=9;i<kline.size();++i)
    {
        double sum = 0;
        for(int j=i-9;j<=i;++j)
        {
            sum += kline[j].ftotalVolume;
        }
        kline[i].volumeAverage10 = sum /10;
    }
}

void DataFile::Corvert()
{
//    for(int i=0;i<kline.size();++i)
//    {
//        QString strtemp = kline[i].totalVolume;
//        strtemp = strtemp.mid(1,strtemp.length());
//        strtemp = strtemp.mid(0,strtemp.length()-1);
//        strtemp.replace(QString(","),QString(""));
//        kline[i].ftotalVolume= strtemp.toInt();
//    }
}


