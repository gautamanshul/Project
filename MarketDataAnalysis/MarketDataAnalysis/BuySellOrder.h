#pragma once
#include "IOrder.h"
#include "OrderUpdate.h"

class ExchangeContext; //Forward declaration.
class BuySellOrder : public IOrder
{
public:
	//Delete the defaul destructor
	BuySellOrder() = delete;
	BuySellOrder(const struct OrderUpdate &order);
	const uint64_t & getOrderId() const { return orderid_; }
	uint64_t getNumShares() const { return numShares_; }
	uint64_t getSequenceNumber() const { return orderSequence; }
	uint64_t getClientId() const { return clientid_; }
	Side getSide() const { return buyOrder_; }
	UpdateType getUpdateType() const { return orderType_;  }
	float getPrice() const { return price_; }
	void setNumShares(uint64_t numOfShares) { numShares_ = numOfShares; }
	void setPrice(float  price) { price_ = price; }

	// More methods can be definted to access other fields
	virtual ~BuySellOrder() = default;
	void run(ExchangeContext& context) override;

protected:
	uint64_t orderid_;
	UpdateType orderType_;
	uint64_t orderSequence;
	uint64_t clientid_;
	Side buyOrder_ = Side::Sell;
	uint64_t numShares_ = 0;
	//std::string symbol_;
	float price_ = 0;
	uint64_t canceledShares = 0;

};

