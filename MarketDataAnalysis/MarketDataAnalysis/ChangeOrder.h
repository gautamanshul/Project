#pragma once
#include "stdafx.h"
#include "IOrder.h"
#include "OrderUpdate.h"
#include <string>
class ExchangeContext;
class ChangeOrder : public IOrder
{
public:
	ChangeOrder() = delete;
	ChangeOrder(const struct OrderUpdate &order);
	virtual ~ChangeOrder() = default;
	void run(ExchangeContext& context) override;

	const uint64_t& getOrderId() const { return orderId_; }
	const uint64_t& getCustomerId() const { return customerId_; }
	const uint64_t getChangedShareVol() const { return numberOfShares_; }
	float getPrice() const { return price_; }
	uint64_t getQuantity() override {
		return  numberOfShares_;
	}

protected:
	uint64_t orderId_ = {};
	uint64_t numberOfShares_ = 0;
	uint64_t customerId_ = 0;
	Side buyOrder_ = Side::Sell;
	float price_ = 0;
};
