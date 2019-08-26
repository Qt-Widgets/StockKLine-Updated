#include "MA.h"

MA::MA(int period, VecPtr vecPtr)
	: sum_(0.0), period_(period), vecPtr_(vecPtr)
{
}

double MA::value()
{
	int size = vecPtr_->size();

	sum_ += (*vecPtr_)[size - 1];
	if (size > period_) {
		sum_ -= (*vecPtr_)[size - 1 - period_];
	}

	return sum_ / period_;
}

void MA::setPeriod(int period)
{
    period_ = period;
}
