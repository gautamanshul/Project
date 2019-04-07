#ifndef CHANGE_ORDER_H
#define CHANGE_ORDER_H

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
	void run(ExchangeContext& context, std::shared_ptr<IOrder>) override;

	const uint64_t& getOrderId() const override { return orderId_; }
	uint64_t getClientId() const override { return customerId_; }
	float getPrice() const { return price_; }
	Side getSide() const override { return buyOrder_; }
	UpdateType getUpdateType() const override { return orderType_; }
	uint64_t getQuantity() override { return  numberOfShares_; }
	uint64_t getSequenceNumber() const override { return orderSequence_; }

protected:
	uint64_t orderId_ = {};
	uint64_t numberOfShares_ = 0;
	uint64_t customerId_ = 0;
	Side buyOrder_ = Side::Sell;
	UpdateType orderType_ = UpdateType::New;
	float price_ = 0;
	uint64_t orderSequence_ = 0;
};

#endif
