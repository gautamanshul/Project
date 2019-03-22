#pragma once

#include<limits>

class ExchangeContext; //Forward declaration

class IOrder {
public:
	IOrder() = delete;
	IOrder(const uint64_t ts)
	{
		timestamp_ = ts;
	}

	virtual ~IOrder() = default;
	virtual void run(ExchangeContext &context) = 0;
	uint64_t getTimestamp() const { return timestamp_; }
protected:
	uint64_t timestamp_ = std::numeric_limits<uint64_t>::max(); //Milliseconds past midnight eastern time.

};
