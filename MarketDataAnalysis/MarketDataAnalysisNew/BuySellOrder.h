#ifndef BUY_SELL_ORDER_H
#define BUY_SELL_ORDER_H

#include "IOrder.h"
#include "OrderUpdate.h"

class ExchangeContext; //Forward declaration.
class BuySellOrder : public IOrder
{
public:
	//Delete the defaul destructor
	BuySellOrder() = delete;
	BuySellOrder(const struct OrderUpdate &order);
	const uint64_t & getOrderId() const override { return orderId_; }
	uint64_t getSequenceNumber() const override { return orderSequence_; }
	uint64_t getClientId() const override { return clientId_; }
	Side getSide() const override { return buyOrder_; }
	UpdateType getUpdateType() const override { return orderType_;  }
	float getPrice() const { return price_; }
	void setNumShares(uint64_t numOfShares) { numShares_ = numOfShares; }
	void setPrice(float  price) { price_ = price; }
	uint64_t getQuantity() override { return  numShares_; }

	// More methods can be definted to access other fields
	virtual ~BuySellOrder() = default;
	void run(ExchangeContext& context, std::shared_ptr<IOrder>) override;
	

protected:
	uint64_t orderId_;
	UpdateType orderType_;
	uint64_t orderSequence_;
	uint64_t clientId_;
	Side buyOrder_ = Side::Sell;
	uint64_t numShares_ = 0;
	//std::string symbol_;
	float price_ = 0;
	uint64_t canceledShares = 0;

};


#endif
