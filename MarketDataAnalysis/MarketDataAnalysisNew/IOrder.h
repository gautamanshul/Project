#ifndef IORDER_H
#define IORDER_H

#include<limits>
#include "OrderUpdate.h"
#include <memory>

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
	virtual void run(ExchangeContext &context, std::shared_ptr<IOrder>) = 0;
	uint64_t getTimestamp() const { return timestamp_; }
	virtual Side getSide() const = 0;
	virtual const uint64_t & getOrderId() const = 0;
	virtual uint64_t getClientId() const = 0;
	virtual uint64_t getSequenceNumber() const = 0;
	virtual UpdateType getUpdateType() const = 0;
	float getPrice () const { return price_; }
	virtual uint64_t getQuantity() = 0;
	
protected:
	uint64_t timestamp_ = std::numeric_limits<uint64_t>::max(); //Milliseconds past midnight eastern time.
	Side side_;
	UpdateType updateType_;
	float price_ = 0;
	
};


#endif
