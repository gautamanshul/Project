#ifndef CANCEL_ORDER_H
#define CANCEL_ORDER_H

#include "IOrder.h"
#include <string>
class ExchangeContext;
class CancelOrder : public IOrder
{
public:
	CancelOrder() = delete;
	CancelOrder(const struct OrderUpdate &order);
	virtual ~CancelOrder() = default;
	void run(ExchangeContext& context, std::shared_ptr<IOrder>) override;
	const uint64_t & getOrderId() const override { return orderId_; }
	Side getSide() const override { return buyOrder_; }
	UpdateType getUpdateType() const override { return orderType_; }
	uint64_t getClientId() const override { return clientId_; }
	uint64_t getSequenceNumber() const override { return orderSequence_; }
	uint64_t getQuantity() override {
		return  cancelledShares_;
	}

protected:
	uint64_t orderId_ = {};
	uint64_t cancelledShares_ = 0;
	uint64_t clientId_ = 0;
	uint64_t orderSequence_ = 0;
	Side buyOrder_ = Side::Sell;
	UpdateType orderType_ = UpdateType::New;
};



#endif
