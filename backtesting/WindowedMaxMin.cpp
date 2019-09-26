#include "WindowedMaxMin.h"

void WindowedMaxMin::update(double value)
{
    if (count_ <= length_) {
        values_[tail_] = value;
        tail_ = tail_ + 1;
        count_++;
    } else {
        values_[head_] = value;
        head_ = (head_ + 1) % length_;
    }

    max_ = 0.0;
    min_ = std::numeric_limits<double>::max();
    int pos = head_;
    for (int c = 0; c < length_; c++) {
        double v = values_[pos];
        if (v < min_) {
            min_ = v;
        } else if (v > max_) {
            max_ = v;
        }
        pos = (pos + 1) % length_;
    }
}

double WindowedMaxMin::getMax()
{
    return max_;
}

double WindowedMaxMin::getMin()
{
    return min_;
}
