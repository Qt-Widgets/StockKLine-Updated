#pragma once

#include <vector>
#include <memory>

using VecPtr = std::shared_ptr<std::vector<double>>;

class MA
{
public:
	MA(int period, VecPtr vecPtr);
	double value();

private:
	VecPtr vecPtr_;
	int period_;
	double sum_;
};