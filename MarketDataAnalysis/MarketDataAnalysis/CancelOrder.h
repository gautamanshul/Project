#pragma once
#include "stdafx.h"
#include "IOrder.h"
#include <string>
class ExchangeContext;
class CancelOrder : public IOrder
{
public:
	CancelOrder() = delete;
	CancelOrder(const struct OrderUpdate &order);
	virtual ~CancelOrder() = default;
	void run(ExchangeContext& context) override;

	const uint64_t& getOrderId() const { return orderId_; }
	const uint64_t& getCustomerId() const { return customerId_; }
	const uint64_t getCanceledShareVol() const { return cancelledShares_; }
	uint64_t getQuantity() override {
		return  cancelledShares_;
	}

protected:
	uint64_t orderId_ = {};
	uint64_t cancelledShares_ = 0;
	uint64_t customerId_ = 0;
};

