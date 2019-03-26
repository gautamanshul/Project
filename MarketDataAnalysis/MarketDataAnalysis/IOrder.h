#pragma once

#include<limits>
#include "OrderUpdate.h"

class ExchangeContext; //Forward declaration

class IOrder {
public:
	IOrder() = delete;
	IOrder(const uint64_t ts, Side side, UpdateType type, float price = 0)
	{
		timestamp_ = ts;
		side_ = side;
		updateType_ = type;
		price_ = price;
	}

	virtual ~IOrder() = default;
	virtual void run(ExchangeContext &context) = 0;
	uint64_t getTimestamp() const { return timestamp_; }
	virtual Side getSide() const { return side_; }
	UpdateType getUpdateType() const { return updateType_; }
	float getPrice () const { return price_; }
	virtual uint64_t getQuantity() = 0;
protected:
	uint64_t timestamp_ = std::numeric_limits<uint64_t>::max(); //Milliseconds past midnight eastern time.
	Side side_;
	UpdateType updateType_;
	float price_ = 0;
};
