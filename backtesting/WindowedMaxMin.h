#pragma once

#include <vector>

class WindowedMaxMin
{
public:
    WindowedMaxMin(int length) :
        length_(length),
        values_(std::vector<double>(length)),
        max_(0.0), min_(std::numeric_limits<double>::max()),
        head_(0), tail_(0), count_(1)
    {}

    void update(double value);
    double getMax();
    double getMin();

private:
    std::vector<double> values_;
    int length_;
    int head_;
    int tail_;
    int count_;
    double max_;
    double min_;
};
